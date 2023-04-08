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

int ft_exit(t_token *token_list, int status, int is_pipe)
{
	t_token			*p;
	int				s;

	p = token_list->next;
	if (!p)
	{
		if (is_pipe)
			exit (status);
		return (status);
	}
	s = ft_atoi(p->token);
	if (is_pipe)
		exit ((unsigned char)s);
	return ((unsigned char)s);
}
