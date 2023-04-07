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

int ft_cd(t_token *token_list, char **env, int is_pipe)
{
	char	*dir;
	t_token	*p;
	int		i;

	p = token_list->next;
	if (p && p->next && p->next->token[0] != '\0')
	{
		write(STDERR_FILENO,"cd : Too many arguments\n", 24);
		if (is_pipe)
			exit(1);
		return (1);
	}
	if (!p)
		dir = ft_getenv("$HOME", env);
	else
		dir = ft_strtrim(p->token, " ");
	i = chdir(dir);
	free (dir);
	if (i == -1)
	{
		perror("cd");
		if (is_pipe)
			exit(1);
		return (1);
	}
	if (is_pipe)
		exit(0);
	return (0);
}
