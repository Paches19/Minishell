/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/14 11:05:52 by adpachec          #+#    #+#             */
/*   Updated: 2022/09/27 10:55:31 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dsize)
{
	size_t	len_dst;
	size_t	len_src;
	size_t	i;

	len_src = ft_strlen(src);
	if (!dsize)
		return (len_src);
	len_dst = ft_strlen(dst);
	i = 0;
	while (src[i] && len_dst + i < dsize - 1)
	{
		dst[len_dst + i] = src[i];
		++i;
	}
	if (len_dst > dsize)
		return (len_src + dsize);
	dst[len_dst + i] = '\0';
	return (len_src + len_dst);
}
