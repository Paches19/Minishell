/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 16:25:04 by adpachec          #+#    #+#             */
/*   Updated: 2022/09/28 12:25:46 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*temp;
	t_list	*aux;

	if (!lst || !del)
		return ;
	temp = *lst;
	while (temp)
	{
		aux = temp->next;
		ft_lstdelone(temp, del);
		temp = aux;
	}
	*lst = NULL;
}
