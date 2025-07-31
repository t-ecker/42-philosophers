#include "./philo.h"

bool	check_meals_eaten(t_philo *philo)
{
	sem_wait(philo->eating_lock);
	bool finishedEating = philo->data->max_meals > 0 && (philo->meal_count >= philo->data->max_meals);
	sem_post(philo->eating_lock);
	if (finishedEating)
	{
		sem_wait(philo->shutdown_lock);
		philo->shutdown = true;
		philo->statusCode = EXIT_MAX_MEALS;
		sem_post(philo->shutdown_lock);
	}
	return (finishedEating);
}

void	*checkOwnDeath(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (!check_shutdown(philo))
	{
		sem_wait(philo->eating_lock);
		if (current_time_in_ms() - philo->last_meal > philo->data->time_to_die)
		{
			write_message("died", philo->data, philo->num, true);
			sem_wait(philo->shutdown_lock);
			philo->shutdown = true;
			philo->statusCode = EXIT_DIED;
			sem_post(philo->shutdown_lock);
			sem_post(philo->eating_lock);
			break ;
		}
		sem_post(philo->eating_lock);
		usleep(philo->data->philo_count * 50);
	}
	return NULL;
}

void	*checkOthersDeath(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	sem_wait(philo->data->terminate);
	sem_wait(philo->shutdown_lock);
	philo->shutdown = true;
	sem_post(philo->shutdown_lock);
	return NULL;
}

bool check_shutdown(t_philo *philo)
{
	bool status;
	sem_wait(philo->shutdown_lock);
	status = philo->shutdown;
	sem_post(philo->shutdown_lock);
	return(status);
}