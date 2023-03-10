/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/14 16:34:42 by adpachec          #+#    #+#             */
/*   Updated: 2022/10/03 10:01:27 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	size_t	sz;
	char	*ptr;

	sz = ft_strlen((char *)s1) + 1;
	ptr = (char *) malloc(sizeof(char) * sz);
	if (!ptr)
		return (NULL);
	ft_memcpy(ptr, s1, sz);
	return (ptr);
}
