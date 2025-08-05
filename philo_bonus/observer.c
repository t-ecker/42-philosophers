#include "./philo.h"

void	check_meals_eaten(t_philo *philo)
{
	sem_wait(philo->eating_lock);
	if (philo->data->max_meals > 0 && (philo->meal_count >= philo->data->max_meals) && !philo->hasFinished)
	{
		sem_post(philo->data->max_meals_sem);
		sem_wait(philo->shutdown_lock);
		philo->shutdown = true;
		philo->hasFinished = true;
		philo->statusCode = EXIT_MAX_MEALS;
		sem_post(philo->shutdown_lock);
	}
	sem_post(philo->eating_lock);
}

void *check_all_meals_eaten(void *args)
{
	t_data *data;
	int finishedEating;
	int i;
	bool res;

	i = -1;
	finishedEating = 0;
	data = args;
	while(++i < data->philo_count)
	{
		sem_wait(data->max_meals_sem);

		sem_wait(data->mainStop_lock);
		res = data->mainStop;
		sem_post(data->mainStop_lock);
		if (res)
			return NULL;
		if (++finishedEating >= data->philo_count)
		{
			i = -1;
			while (++i < data->philo_count)
				sem_post(data->terminate);
			break;
		}
	}
	return NULL;
}

void	*checkOwnDeath(void *arg)
{
	t_philo	*philo;
	int i;

	i = -1;
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
			while(++i < philo->data->philo_count)
				sem_post(philo->data->terminate);
			sem_post(philo->shutdown_lock);
			sem_post(philo->eating_lock);
			break ;
		}
		sem_post(philo->eating_lock);
		usleep(philo->data->philo_count * 50);
	}
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