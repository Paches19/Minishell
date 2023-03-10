/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/14 12:29:38 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/07 13:00:25 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int		i;
	char	*str;

	if (!str)
		return (NULL);
	str = (char *) s;
	i = 0;
	while (str[i])
	{
		if (str[i] == (char) c)
			return (&str[i]);
		++i;
	}
	if (c == '\0')
		return (&str[i]);
	return (NULL);
}
