/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 12:36:02 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/24 13:28:11 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int ft_exit(t_token *token_list, int *status)
{
	t_token	*p;

	p = token_list->next;
	if (!p)
		return (*status);
	*status = ft_atoi(p->token);
	return (*status);
}
