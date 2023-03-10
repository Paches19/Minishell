/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 17:54:09 by adpachec          #+#    #+#             */
/*   Updated: 2022/09/28 12:58:22 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int		len;
	t_list	*temp;

	len = 0;
	if (!lst)
		return (0);
	if (lst->next == NULL)
		return (1);
	temp = lst;
	while (temp->next != NULL)
	{
		++len;
		temp = temp->next;
	}	
	++len;
	return (len);
}
