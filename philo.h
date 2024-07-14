#ifndef PHILO_H
#define PHILO_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct s_data
{
	int philo_count;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int max_meals;
    int stop;
    long start_time;
    pthread_mutex_t stop_m;
    pthread_mutex_t write_m;
    pthread_mutex_t eating;
}   t_data;

typedef struct s_philo
{
	int num;
	int eat_count;
    long last_meal;
    t_data *data;
	pthread_t thread;
	pthread_mutex_t left_f;
	pthread_mutex_t right_f;
}	t_philo;



int	ft_atoi(const char *str);
void *check_stop(void *arg);



#endif