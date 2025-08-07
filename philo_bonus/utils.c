/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 20:28:42 by tomecker          #+#    #+#             */
/*   Updated: 2025/08/07 22:12:10 by tomecker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

long	current_time_in_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	ft_usleep(int time)
{
	long	start_time;

	start_time = current_time_in_ms();
	while (current_time_in_ms() - start_time < time)
		usleep(500);
}

void	write_message(char *str, t_data *data, int num)
{
	sem_wait(data->write);
	printf("%ld %i %s\n",
		current_time_in_ms() - data->start_time, num, str);
	sem_post(data->write);
}

char	*make_unique_semname(char *base, int index)
{
	int		i;
	int		j;
	char	*name;

	i = 0;
	j = 0;
	name = malloc(sizeof(char) * (ft_strlen(base) + 4));
	if (!name)
		return (NULL);
	while (base[i])
		name[j++] = base[i++];
	if (index >= 100)
		name[j++] = '0' + (index / 100 % 10);
	if (index >= 10)
		name[j++] = '0' + (index / 10 % 10);
	name[j++] = '0' + (index % 10);
	name[j] = '\0';
	return (name);
}

void	close_shared_sems(t_data *data)
{
	if (data->forks && data->forks != SEM_FAILED)
		sem_close(data->forks);
	if (data->write && data->write != SEM_FAILED)
		sem_close(data->write);
	if (data->access && data->access != SEM_FAILED)
		sem_close(data->access);
	if (data->terminate && data->terminate != SEM_FAILED)
		sem_close(data->terminate);
	if (data->main_stop_lock && data->main_stop_lock != SEM_FAILED)
		sem_close(data->main_stop_lock);
	if (data->max_meals_sem && data->max_meals_sem != SEM_FAILED)
		sem_close(data->max_meals_sem);
	if (data->death && data->death != SEM_FAILED)
		sem_close(data->death);
}
