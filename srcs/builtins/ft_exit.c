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

int ft_exit(t_token *token_list, int status)
{
	t_token	*p;
	int		s;

	p = token_list->next;
	if (!p)
		return (status);
	s = ft_atoi(p->token);
	if (ft_strcmp(p->token, "0") != 0 && s == 0)
		s = 255;
	return (s);
}
