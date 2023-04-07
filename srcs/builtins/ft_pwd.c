/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 12:30:16 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/29 10:37:06 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int ft_pwd(int is_pipe)
{
	char	dir[1024];

	if (getcwd(dir, sizeof(dir)) == NULL)
	{
		perror("pwd");
		if (is_pipe)
			exit (1);
		return (1);
	}
	else
	{
		ft_putstr_fd(dir, 1);
		ft_putchar_fd('\n', 1);
		if (is_pipe)
			exit (0);
		return (0);
	}	
}
