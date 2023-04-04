/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 12:56:42 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/24 13:28:44 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void swap_tokens(t_token **token_list, t_token *t1, t_token *t2) 
{
    if (t1->prev) 
        t1->prev->next = t2;
    else 
        *token_list = t2;
    if (t2->next)
        t2->next->prev = t1;
    t1->next = t2->next;
    t2->prev = t1->prev;
    t1->prev = t2;
    t2->next = t1;
}

void sort_tokens(t_token **token_list)
{
    t_token *current;
    int 	swap_occurred;

	current = *token_list;
	swap_occurred = 1;
    while (swap_occurred) 
	{
        swap_occurred = 0;
        while (current->next) 
		{
            if (current->type > current->next->type) 
			{
                swap_tokens(token_list, current, current->next);
                current = current->prev;
                swap_occurred = 1;
            }
            current = current->next;
        }
        while (current->prev)
            current = current->prev;
    }
}