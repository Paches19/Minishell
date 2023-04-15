/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 12:36:02 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/29 10:37:21 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_num_exit(const char *str)
{
	int	sign;
	int	result;
	int	i;

	result = 0;
	sign = 1;
	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if ((str[i] == 45 || str[i] == 43))
	{
		if (str[i] == 45)
			sign = -1;
		i++;
	}
	while (str[i] && str[i] >= 48 && str[i] <= 57)
	{
		result = result * 10 + str[i] - 48;
		i++;
	}
	if (str[i])
		return (0);
	return (result * sign);
}

static int	lots_of_args(t_token *t)
{
	int		i;
	t_token	*p;

	i = 0;
	p = t;
	while (p)
	{
		i++;
		p = p->next;
	}
	if (i > 1)
	{
		ft_putstr_fd("minishell: exit: Too many arguments\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}

static void	error_built_exit(char *token)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(token, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
}

int	ft_exit(t_token *token_list, int status, int is_pipe)
{
	t_token	*p;
	int		s;

	p = token_list->next;
	s = status;
	if (!p)
		s = status;
	else if (lots_of_args(p))
		s = 1;
	else
	{
		s = ft_num_exit(p->token);
		if (s == 0 && ft_strcmp(p->token, "0") != 0)
		{
			s = 255;
			error_built_exit(p->token);
		}
	}
	if (is_pipe)
		exit (s);
	return (s);
}
