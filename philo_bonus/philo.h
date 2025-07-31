#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <semaphore.h>
#include <stdbool.h>
#include <signal.h>
#include <string.h>


# define PHILO_MAX 200

typedef enum e_exit_code
{
	EXIT_SUCCESSFUL = 0,
	EXIT_DIED       = 1,
	EXIT_ERROR      = 2,
	EXIT_MAX_MEALS  = 3
}	t_exit_code;

typedef struct s_philo	t_philo;

typedef struct s_data
{
	int				philo_count;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				max_meals;
	int				id;
	long			start_time;
	sem_t			*forks;
	sem_t			*write;
	sem_t			*access;
	sem_t			*terminate;
}	t_data;

typedef struct s_philo
{
	int				num;
	long			last_meal;
	int				meal_count;
	bool			shutdown;
	int				statusCode;
	char 			*semName_eating;
	char 			*semName_shutdown;
	pthread_t 		checkOwnDeath_Thread;
	pthread_t 		checkOthersDeath_Thread;
	sem_t			*eating_lock;
	sem_t			*shutdown_lock;
	t_data			*data;
}	t_philo;

int		ft_atoi(const char *str);
void	write_message(char *str, t_data *data, int num, bool stopWriting);
void	ft_usleep(int time);
long	current_time_in_ms(void);
int		init_philos(t_data *data);
int		init_data(t_data *data, char **argv, int argc);
void	*checkOwnDeath(void *arg);
void	*checkOthersDeath(void *arg);
void	philo_routine(void *arg);
void	cleanup(t_data *data);
char 	*make_unique_semName(char *base, int index);
bool	check_meals_eaten(t_philo *philo);
int		is_all_num(char *str);
bool 	check_shutdown(t_philo *philo);



#endif