/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pminne <pminne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 17:15:20 by pminne            #+#    #+#             */
/*   Updated: 2020/09/25 17:03:45 by pminne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	print_state(t_table *philo, int id, int state)
{
	sem_wait(philo->write->sem_write);
	ft_putnbr_fd((timestamp() - philo->start_program), 1);
	write(1, " ", 1);
	ft_putnbr_fd(id + 1, 1);
	if (state == FORK)
		write(1, " has taken a fork\n", ft_strlen(" has taken a fork\n"));
	else if (state == EATING)
		write(1, " is eating\n", ft_strlen(" is eating\n"));
	else if (state == SLEEPING)
		write(1, " is sleeping\n", ft_strlen(" is sleeping\n"));
	else if (state == THINKING)
		write(1, " is thinking\n", ft_strlen(" is thinking\n"));
	sem_post(philo->write->sem_write);
}

void	print_death(t_table *philo, long t_stamp)
{
	sem_wait(philo->write->sem_write);
	ft_putnbr_fd(t_stamp, 1);
	write(1, " ", 1);
	ft_putnbr_fd(philo->id + 1, 1);
	write(1, " is dead\n", 9);
	sem_post(philo->g_mtx->sem_dead);
}
