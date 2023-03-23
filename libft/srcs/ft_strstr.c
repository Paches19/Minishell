/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 20:13:49 by jutrera-          #+#    #+#             */
/*   Updated: 2023/03/15 20:13:49 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

char	*ft_strstr(const char *haystack, const char *needle)
{
	size_t	i;
	size_t	j;
	size_t	n_len;

	if (!haystack)
		return (0);
	n_len = ft_strlen(needle);
	if (n_len == 0)
		return ((char *)haystack);
	i = 0;
	while ((haystack[i] != '\0'))
	{
		if (haystack[i] == needle[0])
		{
			j = 0;
			while (++j < n_len)
				if (haystack[i + j] != needle[j])
					j = n_len + 1;
			if (j == n_len)
				return ((char *)(haystack + i));
		}
		i++;
	}
	return (0);
}
