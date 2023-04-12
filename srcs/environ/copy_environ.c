/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_environ.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 12:46:20 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/27 13:24:45 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**copy_environ(char **source)
{
	char	**dest;
	int		len;
	int		i;

	len = 0;
	while (source[len])
		len++;
	dest = (char **)ft_calloc(len + 1, sizeof(char *));
	if (!dest)
		return (0);
	i = 0;
	while (source[i])
	{
		dest[i] = ft_strdup(source[i]);
		i++;
	}
	dest[i] = 0;
	return (dest);
}
