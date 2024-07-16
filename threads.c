#include "philo.h"

void	init_fork_mutex(pthread_mutex_t *forks, t_data data)
{
	int	i;

	i = 0;
	while (i < data.philo_count)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
}

void	destroy_forks(pthread_mutex_t *forks, t_data data)
{
	int	i;

	i = 0;
	while (i < data.philo_count)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
}

void	init_threads(t_philo *philo)
{
	int			i;
	pthread_t	t_observ;

	i = 0;
	pthread_mutex_init(&philo->data->stop_m, NULL);
	pthread_mutex_init(&philo->data->write_m, NULL);
	pthread_mutex_init(&philo->data->eating, NULL);
	pthread_create(&t_observ, NULL, observer, philo->data);
	while (i < philo->data->philo_count)
	{
		pthread_create(&philo[i].thread, NULL, philo_routine, &philo[i]);
		i++;
	}
	pthread_join(t_observ, NULL);
}

void	join_threads(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->data->philo_count)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
}
