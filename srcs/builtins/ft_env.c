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

int	ft_env(char ***new_environ, int is_pipe)
{
	int	i;
	int	len;
	int	status;

	status = 0;
	if (!(*new_environ) || !(*new_environ[0]))
		status = -1;
	else
	{
		i = -1;
		while ((*new_environ)[++i])
		{
			len = ft_strlen((*new_environ)[i]);
			if (ft_strcmp((*new_environ)[i] + len - 2, "''"))
			{
				ft_putstr_fd((*new_environ)[i], STDOUT_FILENO);
				ft_putchar_fd('\n', STDOUT_FILENO);
			}
		}
	}
	if (is_pipe)
		exit(status);
	return (status);
}
