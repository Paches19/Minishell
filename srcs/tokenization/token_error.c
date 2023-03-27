/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 13:22:45 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/27 13:23:01 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
		write(2, "minishell: syntax error near unexpected token `", 47);
		c = *token;
		write(2, &c, 1);
		write(2, "\'\n", 2);
	}
	if (err == -1)
		write(2,"minishell: open quotes. Please close quotes if you use \
		them\n", 65);
	return (-1);
}
