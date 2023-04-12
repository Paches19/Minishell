/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 12:30:16 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/29 10:37:06 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	there_are_args(t_token *token_list)
{
	int		i;
	t_token	*p;

	i = 0;
	p = token_list->next;
	while (p)
	{
		i++;
		p = p->next;
	}
	if (i > 0)
	{
		ft_putstr_fd("minishell: pwd: Too many arguments", STDERR_FILENO);
		return (1);
	}
	return (0);
}

int	ft_pwd(t_token *token_list, int is_pipe)
{
	char	dir[1024];
	int		s;

	s = 1;
	if (!there_are_args(token_list))
	{
		if (getcwd(dir, sizeof(dir)) != NULL)
		{
			ft_putstr_fd(dir, STDOUT_FILENO);
			s = 0;
		}
		else
			ft_putstr_fd("minishell: pwd", STDERR_FILENO);
	}
	ft_putchar_fd('\n', STDOUT_FILENO);
	if (is_pipe)
		exit (s);
	return (s);
}
