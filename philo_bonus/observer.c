#include "./philo.h"

bool	check_meals_eaten(t_philo *philo)
{
	sem_wait(philo->eating);
	bool finishedEating = (philo->meal_count >= philo->data->max_meals) && philo->data->max_meals > 0;
	sem_post(philo->eating);
	return (finishedEating);
}

void	*checkDeath(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		sem_wait(philo->eating);
		if (current_time_in_ms() - philo->last_meal > philo->data->time_to_die)
		{
			write_message("died", philo->data, philo->num, true);
			exit(EXIT_DIED);
		}
		sem_post(philo->eating);
		usleep(philo->data->philo_count * 50);
	}
}
