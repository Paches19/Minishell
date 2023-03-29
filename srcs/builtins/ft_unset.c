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
	i = -1;
	while ((*new_environ)[++i] && ft_strncmp((*new_environ)[i], \
	p->token, ft_strlen(p->token)));
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

int ft_unset(t_token *token_list, char ***new_environ)
{
	t_token	*p;
	int		stat;

	p = token_list->next;
	if (!p)
		return (ft_builtins_errors('u'));
	stat = 0;
	while (p && p->type == COMMAND)
	{
		if (!p)
			return (ft_builtins_errors('u'));
		if (ft_strchr(p->token, '='))
			return (ft_builtins_errors('e'));
		stat = ft_delete_var(p, new_environ);
		p = p->next;
	}
	return (stat);
}
