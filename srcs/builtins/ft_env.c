/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 12:32:44 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/29 10:37:25 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int ft_env_in_order(char **new_environ, int len)
{
	int		i;
	int 	j;
	char	*x;
	char	**temp;

	i = -1;
	temp = (char **)ft_calloc(len + 1, sizeof(char *));
	while (new_environ[++i])
		temp[i] = ft_strdup(new_environ[i]);
	i = -1;
	while (temp[++i])
	{
		j = i;
		while (temp[++j])
		{
			if (ft_strcmp(temp[i], temp[j]) > 0)
			{
				x = temp[i];
				temp[i] = temp[j];
				temp[j] = x;
			}
		}
	}
	i = -1;
	while (temp[++i])
		printf("%s\n", temp[i]);
	free_environ(&temp);
	return (0);
}

int ft_env(char ***new_environ)
{
	int	i;
	int	len;

	if (!(*new_environ) || !(*new_environ[0]))
		return (-1);
	i = -1;
	while ((*new_environ)[++i])
	{
		len = ft_strlen((*new_environ)[i]);
		if (ft_strcmp((*new_environ)[i] + len - 2, "''"))
			printf("%s\n", (*new_environ)[i]);
	}
	return (0);
}