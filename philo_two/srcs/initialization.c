/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pminne <pminne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/19 13:37:39 by pminne            #+#    #+#             */
/*   Updated: 2020/09/21 18:48:48 by pminne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void		copy_args(t_table *philo, t_args *args,
	int count)
{
	philo->id = count;
	philo->nb_philo = args->nb_philo;
	philo->time_to_eat = args->time_to_eat;
	philo->time_to_sleep = args->time_to_sleep;
	philo->time_to_starve = args->time_to_starve;
	philo->turns = args->n_time_must_eat;
	if (philo->turns == 0)
		philo->turns = -1;
}

int			init_gbl_semaphore(t_table **philo, t_args *args)
{
	if (((*philo)[0].write->sem_write =
		sem_open((*philo)[0].write->sem_name, O_CREAT, S_IRUSR | S_IWUSR, 1))
			== SEM_FAILED)
		return (error_semaphore(&(*philo)[0], 0));
	if (((*philo)[0].fork->sem_forks =
		sem_open((*philo)[0].fork->sem_name, O_CREAT, S_IRUSR | S_IWUSR,
			args->nb_philo))
				== SEM_FAILED)
		return (error_semaphore(&(*philo)[0], 0));
	if (((*philo)[0].g_mtx->sem_dead =
		sem_open((*philo)[0].g_mtx->dead_name, O_CREAT, S_IRUSR | S_IWUSR, 1))
			== SEM_FAILED)
		return (error_semaphore(&(*philo)[0], 0));
	if (((*philo)[0].g_mtx->sem_done =
		sem_open((*philo)[0].g_mtx->done_name, O_CREAT, S_IRUSR | S_IWUSR, 1))
			== SEM_FAILED)
		return (error_semaphore(&(*philo)[0], 0));
	if (((*philo)[0].g_mtx->sem_meals =
		sem_open((*philo)[0].g_mtx->meals_name, O_CREAT, S_IRUSR | S_IWUSR, 1))
			== SEM_FAILED)
		return (error_semaphore(&(*philo)[0], 0));
	return (0);
}

int			init_philo_semaphore(t_table **philo, t_gbl_var **g_mtx,
	t_args *args)
{
	int		i;

	i = 0;
	while (i < args->nb_philo)
	{
		(*philo)[i].fork = (*philo)[0].fork;
		copy_args(&(*philo)[i], args, i);
		(*philo)[i].write = (*philo)[0].write;
		meal_name((*philo)[i].sem_name, i);
		if (((*philo)[i].sem_meal =
			sem_open((*philo)[i].sem_name, O_CREAT, S_IRUSR | S_IWUSR, 1))
				== SEM_FAILED)
			return (error_semaphore(&(*philo)[0], i));
		sem_unlink((*philo)[i].sem_name);
		(*philo)[i].g_mtx = (*g_mtx);
		i++;
	}
	unlink_semaphore(&(*philo)[0]);
	return (0);
}

int			init_semaphore(t_table **philo, t_args *args, t_gbl_var **g_mtx)
{
	int			i;

	i = 0;
	if (!((*g_mtx) = malloc(sizeof(t_gbl_var))))
	{
		free(*g_mtx);
		return (1);
	}
	(*g_mtx)->dead_name = ft_strdup("/dead");
	(*g_mtx)->sem_dead = NULL;
	(*g_mtx)->done_name = ft_strdup("/done");
	(*g_mtx)->sem_done = NULL;
	(*g_mtx)->meals_name = ft_strdup("/done");
	(*g_mtx)->sem_meals = NULL;
	(*philo)[0].g_mtx = *g_mtx;
	if (init_gbl_semaphore(philo, args))
		return (1);
	if (init_philo_semaphore(philo, g_mtx, args))
		return (1);
	return (0);
}

void		*allocate_philosophers(t_table **philo, t_args *args)
{
	t_write		*write;
	t_fork		*fork;
	t_gbl_var	*g_mtx;

	write = NULL;
	fork = NULL;
	g_mtx = NULL;
	if (!(write = malloc(sizeof(*write))))
		return (error_allocate(philo, write, fork, MALLOC_WRITE));
	if (!(*philo = malloc(sizeof(**philo) * args->nb_philo)))
		return (error_allocate(philo, write, fork, MALLOC_PHILO));
	if (!(fork = malloc(sizeof(*fork))))
		return (error_allocate(philo, write, fork, MALLOC_FORK));
	(*philo)[0].write = write;
	(*philo)[0].write->sem_write = NULL;
	(*philo)[0].fork = fork;
	(*philo)[0].fork->sem_forks = NULL;
	(*philo)[0].fork->sem_name = ft_strdup("/forks");
	(*philo)[0].write->sem_name = ft_strdup("/write");
	if (init_semaphore(philo, args, &g_mtx))
		return (error_allocate(philo, write, fork, ERROR_SEMAPHORE));
	return (*philo);
}
