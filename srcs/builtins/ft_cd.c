/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 12:29:42 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/24 13:27:55 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int ft_cd(t_token *token_list, char **env)
{
	char	*dir;
	t_token	*p;
	int		i;

	p = token_list->next;
	if (!p)
		dir = ft_getenv("HOME", env);
	else
		dir = p->token;
	i = chdir(dir);
	if (i == -1)
		perror("cd");
	return (i);
}