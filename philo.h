/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tecker <tecker@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 16:44:06 by tecker            #+#    #+#             */
/*   Updated: 2024/07/16 16:44:07 by tecker           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

# define PHILO_MAX 200

typedef struct s_philo	t_philo;

typedef struct s_data
{
	int				philo_count;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				max_meals;
	int				stop;
	long			start_time;
	t_philo			*philo;
	pthread_mutex_t	stop_m;
	pthread_mutex_t	write_m;
	pthread_mutex_t	eating;
}	t_data;

typedef struct s_philo
{
	int				num;
	long			last_meal;
	int				meal_count;
	int				eating;
	t_data			*data;
	pthread_t		thread;
	pthread_mutex_t	*left_f;
	pthread_mutex_t	*right_f;
}	t_philo;

int		ft_atoi(const char *str);
int		check_death_status(t_data *data);
void	write_message(char *str, t_data *data, int num);
void	ft_usleep(int time);
long	current_time_in_ms(void);
void	init_philos(t_data *data, t_philo *philo,
			pthread_t *threads, pthread_mutex_t *forks);
void	init_data(t_data *data, char **argv, int argc);
void	*observer(void *arg);
void	*philo_routine(void *arg);
int		join_threads(t_philo *philo);
int		init_threads(t_philo *philo);
void	destroy_forks(pthread_mutex_t *forks, t_data data);
int		init_fork_mutex(pthread_mutex_t *forks, t_data data);

#endif