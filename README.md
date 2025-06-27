☕ Coffee Shop Simulation in C (with Pthreads)
This project is a multithreaded simulation of a coffee shop using the Producer-Consumer problem pattern. It models the interaction between a barista and multiple customers, using semaphores and threads to handle synchronization, customer arrivals, and service.

🧠 Project Concept
A barista serves customers one by one.

A fixed number of chairs (waiting area) limits how many customers can wait at a time.

Customers arrive at fixed intervals.

If the shop is full, the customer leaves.

The barista sleeps if no customers are waiting.

The shop closes after 60 seconds, after which no more customers are accepted.

🔧 Technologies Used
C Programming

Pthreads (POSIX threads)

Semaphores (Custom-built)

UNIX Signals (SIGALRM)

Time functions from <time.h>

🏁 How It Works
Initialization:

A fixed number of customers (NUM_OF_CUSTOMERS)

Limited chairs (CHAIRS) for waiting customers

Custom semaphore struct MySem for synchronization

Threads:

One barista thread

Multiple customer threads

Service Logic:

Customers check for space.

If there’s room, they wait and get served.

If not, they leave immediately.

The barista serves in FIFO order.

Time Tracking:

Each customer's:

Arrival time

Waiting time

Start and end of service

Service duration

📦 How to Compile and Run
Compile:
bash
Copy
Edit
gcc -o coffee_shop coffee_shop.c -lpthread
Run:
bash
Copy
Edit
./coffee_shop
⚙️ Configuration Parameters
Parameter	Description	Default Value
NUM_OF_CUSTOMERS	Total number of customers to simulate	5
CHAIRS	Number of waiting chairs in the shop	2
SHOP_CLOSE_IN	Time (in seconds) before shop closes	60
SERVICE_TIME	Time taken by barista to make one coffee	5
ARRIVAL_TIME	Time gap between customer arrivals	3

You can edit these values at the top of the coffee_shop.c file.

📊 Output Example
The program prints log messages like:

bash
Copy
Edit
Barista is sleeping zzzz....
Customer 0 entered the coffee shop and is waiting for service.
Barista is currently serving a customer.
Customer 1 entered the coffee shop and is waiting for service.
Customer 2 leaves because the coffee shop is full.
Customer 0 has been served and leaves the coffee shop.
...
And shows a table:

sql
Copy
Edit
Customer   Arrival Time  Waiting Time(sec)   Servive Start time  Service End Time  Service Duration
0          01:30 PM       0                  01:30 PM             01:30 PM         5
1          01:30 PM       5                  01:30 PM             01:30 PM         5
📌 Notes
The simulation uses custom semaphores (MySem) with condition variables and mutexes.

The SIGALRM is used to simulate shop closure after 60 seconds.

The time report is shown at the end for each customer.

🧑‍💻 Author
Kissa Zehra
Undergraduate Student, FAST NUCES
Simulation created as a learning project for understanding multithreading and synchronization in C.

