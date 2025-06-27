# ☕ Coffee Shop Simulation in C (with Pthreads)

This project simulates a multithreaded coffee shop where a single barista serves multiple customers using custom semaphores and POSIX threads. It demonstrates concepts of synchronization, producer-consumer problems, and real-time simulation in C.

---

## 🧠 Project Concept

- One **barista** serves coffee to customers one by one.
- Only **2 chairs** are available in the waiting area.
- **5 customers** arrive at the shop every 3 seconds.
- If the shop is full, customers leave without service.
- The **barista sleeps** if there are no customers.
- The **shop automatically closes after 60 seconds**, allowing the barista to finish serving only the last customer.

---

## 🔧 Technologies Used

- C Programming Language
- Pthreads (`pthread.h`)
- Custom-built semaphores
- UNIX Signals (`signal.h`)
- Time manipulation with `time.h`

---

## 📁 File Structure

