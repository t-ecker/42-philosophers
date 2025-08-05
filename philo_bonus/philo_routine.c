#include "./philo.h"

void terminate(t_philo *philo, t_exit_code exitCode)
{
	// cleanup(philo->data);
	sem_close(philo->data->forks);
	sem_close(philo->data->write);
	sem_close(philo->data->access);
	sem_close(philo->data->terminate);
	sem_close(philo->data->mainStop_lock);
	sem_close(philo->data->max_meals_sem);

	if (philo->checkOwnDeath_Thread)
		pthread_join(philo->checkOwnDeath_Thread, NULL);
	if (philo->routine_Thread)
		pthread_join(philo->routine_Thread, NULL);
	
	if (philo->semName_eating)
	{
		sem_close(philo->eating_lock);
		sem_unlink(philo->semName_eating);
		free(philo->semName_eating);
		philo->semName_eating = NULL;
	}
	if (philo->semName_shutdown)
	{
		sem_close(philo->shutdown_lock);
		sem_unlink(philo->semName_shutdown);
		free(philo->semName_shutdown);
		philo->semName_shutdown = NULL;
	}
	if (philo->statusCode == EXIT_DIED || philo->statusCode == EXIT_MAX_MEALS)
		exitCode = philo->statusCode;
	exit(exitCode);
}

void	routine(t_philo *philo)
{
	write_message("is thinking", philo->data, philo->num, false);
	sem_wait(philo->data->access);
	sem_wait(philo->data->forks);
	write_message("has taken a fork", philo->data, philo->num, false);
	sem_wait(philo->data->forks);
	write_message("has taken a fork", philo->data, philo->num, false);
	sem_wait(philo->eating_lock);
	write_message("is eating", philo->data, philo->num, false);
	philo->last_meal = current_time_in_ms();
	philo->meal_count++;
	sem_post(philo->eating_lock);
	ft_usleep(philo->data->time_to_eat);
	sem_post(philo->data->forks);
	sem_post(philo->data->forks);
	sem_post(philo->data->access);
	check_meals_eaten(philo);
	if (check_shutdown(philo))
		return ;
	write_message("is sleeping", philo->data, philo->num, false);
	ft_usleep(philo->data->time_to_sleep);
}

void *routine_Thread(void *args)
{
	t_philo *philo;

	philo = args;
	while (!check_shutdown(philo))
		routine(philo);
	return NULL;
}

int init_philo(t_data *data, t_philo *philo)
{
	memset(philo, 0, sizeof(t_philo));
	philo->num = data->id;
	philo->semName_eating = make_unique_semName("eating_", philo->num);
	philo->semName_shutdown = make_unique_semName("shutdown_", philo->num);
	if (!philo->semName_eating || !philo->semName_shutdown)
		return (1);
	sem_unlink(philo->semName_eating);
	sem_unlink(philo->semName_shutdown);
	philo->eating_lock = sem_open(philo->semName_eating, O_CREAT | O_EXCL, 0644, 1);
	philo->shutdown_lock = sem_open(philo->semName_shutdown, O_CREAT | O_EXCL, 0644, 1);
	if (philo->eating_lock == SEM_FAILED || philo->shutdown_lock == SEM_FAILED)
		return (1);
	philo->data = data;
	philo->last_meal = current_time_in_ms();
	philo->data->start_time = current_time_in_ms();
	
	return (0);
}

void	philo_routine(void *arg)
{
	t_data *data;
	t_philo	philo;
	
	data = (t_data *)arg;
	if (init_philo(data, &philo))
	{
		philo.data = data;
		terminate(&philo, EXIT_ERROR);
	}
	if (pthread_create(&philo.checkOwnDeath_Thread, NULL, checkOwnDeath, &philo) != 0)
		terminate(&philo, EXIT_ERROR);
	if (pthread_create(&philo.routine_Thread, NULL, routine_Thread, &philo) != 0)
	{
		sem_wait(philo.shutdown_lock);
		philo.shutdown = true;
		sem_post(philo.shutdown_lock);
		pthread_join(philo.checkOwnDeath_Thread, NULL);
		philo.checkOwnDeath_Thread = 0;
		terminate(&philo, EXIT_ERROR);
	}
	sem_wait(data->terminate);
	sem_wait(philo.shutdown_lock);
	philo.shutdown = true;
	sem_post(philo.shutdown_lock);
	terminate(&philo, EXIT_CLOSED);
}
