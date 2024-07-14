#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct s_data
{
	int balance;
	pthread_mutex_t mutex;
	int deposit1;
	int deposit2;
} t_data;

void write_balance(t_data *data, int new_balance)
{
  usleep(550000);
  data->balance = new_balance;
}

int read_balance(t_data *data)
{
  usleep(550000);
  return data->balance;
}

void* deposit(void *arg)
{
  t_data *data = (t_data *)arg;
  int amount;

  // Determine which deposit amount to use
  if (pthread_self() == pthread_self()) {
    amount = data->deposit1;
  } else {
    amount = data->deposit2;
  }

  pthread_mutex_lock(&data->mutex);

  int account_balance = read_balance(data);

  account_balance += amount;

  write_balance(data, account_balance);

  pthread_mutex_unlock(&data->mutex);

  return NULL;
}

int main()
{
  t_data data;
  data.balance = 0;
  data.deposit1 = 300;
  data.deposit2 = 200;

  pthread_mutex_init(&data.mutex, NULL);

  int before = read_balance(&data);
  printf("Before: %d\n", before);

  pthread_t thread1, thread2;

  pthread_create(&thread1, NULL, deposit, (void*)&data);
  pthread_create(&thread2, NULL, deposit, (void*)&data);

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  pthread_mutex_destroy(&data.mutex);

  int after = read_balance(&data);
  printf("After: %d\n", after);

  return 0;
}
