/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pminne <pminne@42lyon.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 17:15:11 by pminne            #+#    #+#             */
/*   Updated: 2020/09/07 17:15:12 by pminne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void			print_value(t_args *philo)
{
	printf("numbers of philo : %d\n", philo->nb_philo);
	printf("time_to_die : %d\n", philo->time_to_starve);
	printf("time to eat : %d\n", philo->time_to_eat);
	printf("time to sleep : %d\n", philo->time_to_sleep);
	printf("numbers of time must eat : %d\n", philo->n_time_must_eat);
}

int				ft_is_number(char *s)
{
	int i;

	i = 0;
	while (s[i])
	{
		if (s[i] < '0' && s[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

int				ft_atoi(char *s)
{
	int	neg;
	int	nb;
	int	i;

	nb = 0;
	neg = 1;
	i = 0;
	while (s[i] == '-' || s[i] == '+')
	{
		if (s[i] == '-')
			neg *= -1;
		i++;
	}
	if (neg == -1)
		return (-1);
	if (ft_is_number(&s[i]))
		return (-1);
	while (s[i])
	{
		nb = (nb * 10) + s[i] - 48;
		i++;
	}
	return (nb == 0 ? -1 : nb);
}

unsigned int	ft_strlen(char *s)
{
	int i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

static void		*perror_parsing(char *msg_error, t_args *args)
{
	write(1, msg_error, ft_strlen(msg_error));
	if (args)
		free(args);
	return (NULL);
}

static t_args	*fifth_args(char *av, t_args *args)
{
	args->n_time_must_eat = ft_atoi(av);
	if (args->n_time_must_eat == -1)
		return (perror_parsing("Time_eat must be positive (zero out)\n", args));
	return (args);
}

t_args			*parsing(int ac, char **av)
{
	t_args	*args;

	args = NULL;
	if (ac < 5 || ac > 6)
		return (perror_parsing("Should have only 4 or 5 arguments\n", args));
	if (!(args = malloc(sizeof(t_args))))
		return (perror_parsing("Error malloc args\n", NULL));
	args->nb_philo = ft_atoi(av[1]);
	if (args->nb_philo == -1)
		return (perror_parsing("Nb_philo must be positive (zero out)\n", args));
	args->time_to_starve = ft_atoi(av[2]);
	if (args->time_to_starve == -1)
		return (perror_parsing("Time_starve -> positive (zero out)\n", args));
	args->time_to_eat = ft_atoi(av[3]);
	if (args->time_to_eat == -1)
		return (perror_parsing("Time_to_eat not positive (zero out)\n", args));
	args->time_to_sleep = ft_atoi(av[4]);
	if (args->time_to_sleep == -1)
		return (perror_parsing("Time_sleep not positive (zero out)\n", args));
	if (av[5])
		return (fifth_args(av[5], args));
	else
		args->n_time_must_eat = 0;
	return (args);
}
