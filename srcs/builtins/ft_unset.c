/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 12:35:26 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/29 10:37:15 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	ft_delete_var(t_token *p, char ***new_environ)
{
	int			stat;
	int			i;
	char		*temp;

	stat = 0;
	i = 0;
	p->token = ft_strjoin2(p->token, "=");
	while ((*new_environ)[i] && ft_strncmp((*new_environ)[i], \
	p->token, ft_strlen(p->token)))
		i++;
	if ((*new_environ)[i])
	{
		temp = (*new_environ)[i];
		while ((*new_environ)[i])
		{
			(*new_environ)[i] = (*new_environ)[i + 1];
			i++;
		}
		free((*new_environ)[i]);
		free(temp);
	}
	stat++;
	return (stat);
}

static int	ft_unset_errors(char e)
{
	if (e == 'u')
		ft_putstr_fd("minishell: unset: not enough arguments\n", STDERR_FILENO);
	if (e == 'e')
		ft_putstr_fd("minishell: unset: bad identifier\n", STDERR_FILENO);
	return (1);
}

int	ft_unset(t_token *token_list, char ***new_environ, int is_pipe)
{
	t_token	*p;
	int		stat;

	p = token_list->next;
	if (!p)
	{
		if (is_pipe)
			exit (ft_unset_errors('u'));
		return (ft_unset_errors('u'));
	}
	stat = 0;
	while (p)
	{
		if (ft_strchr(p->token, '=')
			|| (!ft_isalpha(p->token[0]) && p->token[0] != '_')
			|| ft_strlen(p->token) == 0)
			stat = ft_unset_errors('e');
		else
			stat = ft_delete_var(p, new_environ);
		p = p->next;
	}
	if (is_pipe)
		exit(stat);
	return (stat);
}
