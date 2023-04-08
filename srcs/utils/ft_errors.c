/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_errors.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 12:53:17 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/24 13:27:28 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	error_cmd(int err)
{
	perror(strerror(err));
	// exit(err);
}

void	exit_error(int err)
{
	perror("error\n");
	exit(err);
}

int	exit_error_token(int err, char *token)
{
	char	c;
	
	if (err == -2)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
		c = *token;
		write(2, &c, 1);
		write(2, "\'\n", 2);
	}
	if (err == -1)
		ft_putstr_fd("minishell: open quotes. Please close quotes if you use them\n", STDERR_FILENO);
	return (-1);
}
