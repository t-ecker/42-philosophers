#include "philo.h"

int	check_death_status(t_data *data)
{
	int	status;

	pthread_mutex_lock(&data->stop_m);
	status = data->stop;
	pthread_mutex_unlock(&data->stop_m);
	return (status);
}

void	routine(t_philo *philo)
{
	write_message("is thinking", philo->data, philo->num);
	pthread_mutex_lock(philo->left_f);
	write_message("has taken a fork", philo->data, philo->num);
	if (philo->data->philo_count == 1)
	{
		ft_usleep(philo->data->time_to_die);
		pthread_mutex_unlock(philo->left_f);
		return ;
	}
	pthread_mutex_lock(philo->right_f);
	write_message("has taken a fork", philo->data, philo->num);
	philo->eating = 1;
	write_message("is eating", philo->data, philo->num);
	pthread_mutex_lock(&philo->data->eating);
	philo->last_meal = current_time_in_ms();
	philo->meal_count++;
	pthread_mutex_unlock(&philo->data->eating);
	ft_usleep(philo->data->time_to_eat);
	philo->eating = 0;
	pthread_mutex_unlock(philo->right_f);
	pthread_mutex_unlock(philo->left_f);
	write_message("is sleeping", philo->data, philo->num);
	ft_usleep(philo->data->time_to_sleep);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->num % 2 == 0)
		ft_usleep((philo->data->time_to_die
				- (philo->data->time_to_die % 100)) / 2);
	while (check_death_status(philo->data) == 0)
	{
		routine(philo);
		if (philo->data->time_to_die
			> philo->data->time_to_eat + philo->data->time_to_sleep + 100
			&& philo->data->time_to_die < 990)
			ft_usleep(100);
	}
	return (NULL);
}
