/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_heredoc_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 12:32:47 by jutrera-          #+#    #+#             */
/*   Updated: 2023/04/13 12:32:47 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handler_ctrl_c(int sig)
{
	(void)sig;
	exit (0);
}

void	ft_message_ctrld(char *s)
{
	ft_putstr_fd("minishell: warning: here", STDOUT_FILENO);
	ft_putstr_fd("-document delimited by end-of", STDOUT_FILENO);
	ft_putstr_fd("-file at this line (wanted '", STDOUT_FILENO);
	ft_putstr_fd(s, STDOUT_FILENO);
	ft_putstr_fd("')\n", STDOUT_FILENO);
}

void	ft_error_fork(void)
{
	perror("couldn't fork");
	exit(EXIT_FAILURE);
}

void	eliminate_quotes(char **s)
{
	int		i;
	int		len;
	char	*x;

	len = ft_strlen(*s) - 2;
	x = (char *)ft_calloc((len + 1), sizeof(char));
	if (!x)
		exit(1);
	i = 0;
	while (i < len)
	{
		x[i] = (*s)[i + 1];
		i++;
	}
	free(*s);
	*s = x;
}
