# philosophers - 42 School

### 🏆 Score: **100/100**

## 📚 About the Project

**philosophers** is a concurrency project based on the classic *Dining Philosophers problem*, implemented in C using threads and mutexes. It simulates philosophers sitting at a table with one fork between each pair. Each philosopher alternates between thinking, eating, and sleeping, and must pick up both adjacent forks to eat. The main challenge is to coordinate access to these shared forks in a way that avoids deadlocks and starvation

### 🧠 My Approach

1. **Thread per Philosopher**: Each philosopher is represented by a thread that cycles through thinking, eating, and sleeping
2. **Forks as Mutexes**: Forks are implemented as mutexes placed between philosophers to ensure mutual exclusion when eating
3. **Avoiding Deadlocks**: A specific fork-picking strategy is used to prevent circular wait and deadlocks
4. **Monitoring Thread**: A separate thread continuously checks if any philosopher has died due to starvation or if all philosophers have eaten the required number of times (if specified)

### 📚 What I Learned
- **Multithreading and Synchronization in C**: Creating/Managing threads and using mutexes to handle access to shared resources

## 🚀 Usage

### 1️⃣ Clone the Repository

```bash
git clone https://github.com/yourusername/42-philosophers.git
cd philosophers
```

### 2️⃣ Compile the Program

```bash
make
```

### 3️⃣ Run the Simulation

```bash
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
```

Example:

```bash
./philo 5 800 200 200
```

Where:
- `number_of_philosophers`: number of philosopher threads to create
- `time_to_die`: time (in ms) a philosopher can live without eating
- `time_to_eat`: time (in ms) it takes to eat
- `time_to_sleep`: time (in ms) to sleep
- `[number_of_times_each_philosopher_must_eat]`: optional; simulation ends when each philosopher has eaten this many times

## 🏁 Conclusion

**philosophers** is an introduction to concurrency and synchronization in C. It showcases how careful design is necessary to avoid deadlocks and race conditions in multi-threaded applications
