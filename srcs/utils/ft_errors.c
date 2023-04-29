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

void	cmd_error(int err)
{
	perror(strerror(err));
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
		ft_putstr_fd("minishell: syntax error near ", STDERR_FILENO);
		ft_putstr_fd("unexpected token `", STDERR_FILENO);
		c = *token;
		if (c != ' ')
			write(2, &c, 1);
		else
			ft_putstr_fd("newline", STDERR_FILENO);
		write(2, "\'\n", 2);
	}
	if (err == -1)
	{
		ft_putstr_fd("minishell: open quotes. Please close ", STDERR_FILENO);
		ft_putstr_fd("quotes if you use them\n", STDERR_FILENO);
	}
	return (-1);
}

int	export_errors(char *s)
{
	int	e;
	int	i;

	e = 0;
	if (s[0] == '=')
	{
		ft_putstr_fd("minishell: export: bad identifier\n", STDERR_FILENO);
		e = 1;
	}
	else
	{
		i = 0;
		while (s[i] && s[i] != '=' && e == 0)
		{
			if (!ft_isalpha(s[i]) && s[i] != '_')
			{
				ft_putstr_fd("minishell: export: bad identifier\n", \
				STDERR_FILENO);
				e = 1;
			}			
			i++;
		}
	}
	return (e);
}
