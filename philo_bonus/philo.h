/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 20:28:33 by tomecker          #+#    #+#             */
/*   Updated: 2025/08/07 22:29:22 by tomecker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <semaphore.h>
# include <stdbool.h>
# include <signal.h>
# include <string.h>
# include <sys/wait.h>
# include <fcntl.h>

# define PHILO_MAX 200

typedef enum e_exit_code
{
	EXIT_CLOSED	= 0,
	EXIT_DIED = 1,
	EXIT_ERROR = 2,
	EXIT_MAX_MEALS = 3
}	t_exit_code;

typedef struct s_philo	t_philo;

typedef struct s_data
{
	int				philo_count;
	int				time_to_die;
	int				time_to_eat;
	int				max_meals;
	int				time_to_sleep;
	int				id;
	long			start_time;
	bool			shutdown;
	sem_t			*finished_eating;
	sem_t			*cleanup_mutex;
	sem_t			*fork_pool;
	sem_t			*print_mutex;
	sem_t			*fork_guard;
	sem_t			*terminate;
	sem_t			*death_mutex;
}	t_data;

typedef struct s_philo
{
	int				num;
	long			last_meal;
	int				meal_count;
	bool			has_finished;
	int				status_code;
	char			*semname_eating;
	char			*semname_shutdown;
	pthread_t		check_own_death_thread;
	pthread_t		routine_thread;
	sem_t			*eating_mutex;
	sem_t			*shutdown_mutex;
	t_data			*data;
}	t_philo;

int		ft_atoi(const char *str);
void	write_message(char *str, t_data *data, int num);
void	ft_usleep(int time);
long	current_time_in_ms(void);
int		init_philos(t_data *data);
int		init_data(t_data *data, char **argv, int argc);
void	*check_own_death(void *arg);
void	philo_routine(void *arg);
void	cleanup(t_data *data);
char	*make_unique_semname(char *base, int index);
void	check_meals_eaten(t_philo *philo);
int		is_all_num(char *str);
bool	check_shutdown(t_philo *philo);
void	*check_all_meals_eaten(void *args);
void	close_shared_sems(t_data *data);
size_t	ft_strlen(char const *src);

#endif