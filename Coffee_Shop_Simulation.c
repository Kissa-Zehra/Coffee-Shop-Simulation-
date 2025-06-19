#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <signal.h>
#include <stdlib.h>

#define NUM_OF_CUSTOMERS 5
#define CHAIRS 2
#define SHOP_CLOSE_IN 60 // barista will close shop in 1 min(60 sec)
#define SERVICE_TIME 5	 // Barista will make coffee in 5 sec and serve it
#define ARRIVAL_TIME 3	 // Customers will arrive at shop with 3 sec gap

typedef struct
{
	time_t arrival_time;
	time_t waiting_time; // will be calculated in sec
	time_t start_time;	 // start of service time
	time_t end_time;	 // end of service time
	time_t service_time; // duration of service
} TimeInfo;

typedef struct
{
	int value;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
} MySem;

TimeInfo time_report[NUM_OF_CUSTOMERS];

int my_sem_init(MySem *sem, int i)
{
	if (i < 0)
	{
		return -1;
	}

	sem->value = i;
	pthread_mutex_init(&sem->mutex, NULL);
	pthread_cond_init(&sem->cond, NULL);

	return 0;
}

int my_sem_wait(MySem *sem)
{
	pthread_mutex_lock(&sem->mutex);

	while (sem->value <= 0)
	{
		pthread_cond_wait(&sem->cond, &sem->mutex);
	}
	sem->value--;

	pthread_mutex_unlock(&sem->mutex);

	return 0;
}

int my_sem_signal(MySem *sem)
{
	pthread_mutex_lock(&sem->mutex);

	sem->value++;
	pthread_cond_signal(&sem->cond);

	pthread_mutex_unlock(&sem->mutex);

	return 0;
}

MySem customers;
MySem baristas;
MySem mutex;
int numWaiting;
bool shopOpen = true; // used to close the shop
int entered = 0;	  // number of customers entered
pthread_t baristaThread;

void *barista(void *arg)
{
	while (1)
	{
		my_sem_wait(&customers);
		my_sem_wait(&mutex);
		numWaiting--;
		my_sem_signal(&baristas);
		my_sem_signal(&mutex);
		if (shopOpen)
		{
			printf("Barista is currently serving a customer.\n");

			sleep(SERVICE_TIME);
		}
	}

	printf("Barista is closing the coffee shop and going home.\n");
	pthread_exit(NULL);
}
void *customer(void *arg)
{
	int id = *((int *)arg);
	TimeInfo *timeInfo = &time_report[id];
	entered++;
	my_sem_wait(&mutex);
	time(&timeInfo->arrival_time);
	if (numWaiting < CHAIRS)
	{
		numWaiting++;
		printf("Customer %d entered the coffee shop and is waiting for service.\n", id);
		my_sem_signal(&customers);
		my_sem_signal(&mutex);
		my_sem_wait(&baristas);
		if (!shopOpen)
		{
			printf("Customer %d left the shop without being served.\n", id);
			pthread_exit(NULL);
		}
		time(&timeInfo->start_time);
		sleep(SERVICE_TIME);
		printf("Customer %d has been served and leaves the coffee shop.\n", id);
		time(&timeInfo->end_time);
		timeInfo->waiting_time = timeInfo->start_time - timeInfo->arrival_time;
		timeInfo->service_time = timeInfo->end_time - timeInfo->start_time;
	}
	else
	{
		printf("Customer %d leaves because the coffee shop is full.\n", id);
		my_sem_signal(&mutex);
	}
	pthread_exit(NULL);
}
void handle_shop_closure(int sig)
{
	printf("The coffee shop's timing is up . Barista is serving last customer \nRemaining customer can leave.\n");
	shopOpen = false;
}
int main()
{
	pthread_t customer_threads[NUM_OF_CUSTOMERS];
	int ids[NUM_OF_CUSTOMERS];
	printf("Barista is sleeping zzzz....\n");
	my_sem_init(&customers, 0);
	my_sem_init(&baristas, 0);
	my_sem_init(&mutex, 1);

	pthread_create(&baristaThread, NULL, barista, NULL);

	signal(SIGALRM, handle_shop_closure);
	alarm(SHOP_CLOSE_IN);
	for (int i = 0; i < NUM_OF_CUSTOMERS; i++)
	{
		if (shopOpen == false)
		{
			break;
		}
		ids[i] = i;
		pthread_create(&customer_threads[i], NULL, customer, &ids[i]);

		sleep(ARRIVAL_TIME);
	}
	for (int i = 0; i < entered; i++)
	{
		pthread_join(customer_threads[i], NULL);
	}
	printf("The coffee shop is closing.\n");

	printf("\n\nCustomer Time Information:\n\n");
	char arrival_time_str[9], start_time_str[9], end_time_str[9];
	printf("Customer   Arrival Time  Waiting Time(sec)   Servive Start time  Service End Time  Service Duration\n");
	for (int i = 0; i < entered; i++)
	{
		TimeInfo *info = &time_report[i];

		strftime(arrival_time_str, 9, "%I:%M %p", localtime(&info->arrival_time));
		strftime(start_time_str, 9, "%I:%M %p", localtime(&info->start_time));
		strftime(end_time_str, 9, "%I:%M %p", localtime(&info->end_time));
		printf("%d            %s         %ld                    %s        %s              %ld \n", i, arrival_time_str, info->waiting_time, start_time_str, end_time_str, info->service_time);
	}
	return 0;
}