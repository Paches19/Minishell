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

int	ft_builtins_errors(char e)
{
	if (e == 'u')
		printf("unset: not enough arguments\n");
	if (e == 'e')
		printf("export: bad identifier\n");
	return (1);
}

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
