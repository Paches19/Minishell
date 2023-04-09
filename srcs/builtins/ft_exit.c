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
		ft_putstr_fd("exit: Too many arguments\n", STDERR_FILENO);
		return (1);
	}
	return (0);
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
		s = ft_atoi(p->token);
	if (is_pipe)
		exit ((unsigned char)s);
	return ((unsigned char)s);
}
