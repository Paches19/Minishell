/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 19:12:07 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/11 19:19:11 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	
	if (!s1 || !s2)
	 	return (1);
	i = 0;
	
	while (s1[i])
	{
		if (s1[i] != s2[i])
			return ((unsigned char *)s1 - (unsigned char *)s2);
		++i;
	}
	if (s1[i] == '\0' && s2[i] == '\0')
		return (0);
	return ((unsigned char *)s2 - (unsigned char *)s1);
	
	// while (s1[i] && s2[i])
	// {
	// 	if ((unsigned char) s1[i] > (unsigned char) s2[i])
	// 		return (1);
	// 	else if ((unsigned char) s1[i] < (unsigned char) s2[i])
	// 		return (-1);
	// 	else if (s1[i] == '\0' || s2[i] == '\0')
	// 		return (0);
	// 	++i;
	// }
	// return (0);
}
