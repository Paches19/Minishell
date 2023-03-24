/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 12:27:51 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/24 13:27:58 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	ft_printable_token(t_token *p)
{
	return (!(p->type == INPUT_REDIRECT || p->type == HEREDOC_REDIRECT || \
	p->type == PIPE ||p->type == OUTPUT_REDIRECT ||p->type == APPEND_REDIRECT));
}

int ft_echo(t_token *token_list)
{
	t_token	*p;
	int		nl;

	p = token_list->next;
	if (!p || !(!ft_strcmp(p->token, "-n") && ft_strlen(p->token) == 2))
		nl = 1;
	else
	{
		while ((!ft_strcmp(p->token, "-n") && ft_strlen(p->token) == 2))
			p = p->next;
		nl = 0;
	}
	while (p && ft_printable_token(p))
	{
		ft_putstr_fd(p->token, 1);
		p = p->next;
		if (p && p->token)
			write(1, " ", 1);
	}
	if (nl == 0)
		printf("\x1B[30m\x1B[47m%%\x1B[0m\x1B[0m\n");
	else
		write(1, "\n", 1);
	return (0);
}