#include "./philo.h"

void	routine(t_philo *philo)
{
	sem_wait(philo->data->forks);
	write_message("has taken a fork", philo->data, philo->num, false);
	sem_wait(philo->data->forks);
	write_message("has taken a fork", philo->data, philo->num, false);
	sem_wait(philo->eating);
	write_message("is eating", philo->data, philo->num, false);
	philo->last_meal = current_time_in_ms();
	philo->meal_count++;
	sem_post(philo->eating);
	ft_usleep(philo->data->time_to_eat);
	sem_post(philo->data->forks);
	sem_post(philo->data->forks);
	write_message("is sleeping", philo->data, philo->num, false);
	ft_usleep(philo->data->time_to_sleep);
	write_message("is thinking", philo->data, philo->num, false);
}

void	philo_routine(void *arg)
{
	t_philo	*philo;
	pthread_t monitorThread;

	philo = (t_philo *)arg;
	// sem_wait(philo->data->allCreated);
			// write(1, "tt\n", 3);

	philo->last_meal = current_time_in_ms();
	// philo->last_meal = 0;
	if (pthread_create(&monitorThread, NULL, checkDeath, philo) != 0)
		exit(EXIT_ERROR);
	// pthread_detach(monitorThread);
	while (1)
	{
		routine(philo);
		if (check_meals_eaten(philo))
		{
			exit(EXIT_MAX_MEALS);
		}
		// usleep(100);
	}
}