/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 12:45:02 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/24 13:28:27 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_getenv(char *var_name, char **env)
{
	int			i;
	int			j;
	const int	var_len = ft_strlen(++var_name);

	if (!env || !var_name)
		return (NULL);
	i = -1;
	while (env[++i])
	{
		if  (!ft_strncmp(var_name, env[i], var_len))
		{
			j = -1;
			while (env[i][++j] && env[i][j] != '=');
			if (var_len == j)
				return (ft_substr(env[i], j + 1, ft_strlen(env[i] - j + 1)));
		}
	}
	return (NULL);
}
