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

int ft_pwd(void)
{
	char	dir[1024];

	if (getcwd(dir, sizeof(dir)) == NULL)
	{
		perror("pwd");
		return (1);
	}
	else
	{
		printf("%s\n", dir);
		return (0);
	}	
