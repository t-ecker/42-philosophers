/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 20:28:42 by tomecker          #+#    #+#             */
/*   Updated: 2025/08/07 22:20:13 by tomecker         ###   ########.fr       */
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
	sem_wait(data->print_mutex);
	printf("%ld %i %s\n",
		current_time_in_ms() - data->start_time, num, str);
	sem_post(data->print_mutex);
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
	if (data->fork_pool && data->fork_pool != SEM_FAILED)
		sem_close(data->fork_pool);
	if (data->print_mutex && data->print_mutex != SEM_FAILED)
		sem_close(data->print_mutex);
	if (data->fork_guard && data->fork_guard != SEM_FAILED)
		sem_close(data->fork_guard);
	if (data->terminate && data->terminate != SEM_FAILED)
		sem_close(data->terminate);
	if (data->cleanup_mutex && data->cleanup_mutex != SEM_FAILED)
		sem_close(data->cleanup_mutex);
	if (data->finished_eating && data->finished_eating != SEM_FAILED)
		sem_close(data->finished_eating);
	if (data->death_mutex && data->death_mutex != SEM_FAILED)
		sem_close(data->death_mutex);
}
