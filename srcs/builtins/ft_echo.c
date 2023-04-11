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

static void	ft_write_simple(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (!ft_is_quote(s[i]))
			ft_putchar_fd(s[i], STDOUT_FILENO);
		i++;
	}
}

static void	ft_write_echo(char *s)
{
	int	i;

	if (!ft_strcmp(s, "$") && ft_strlen(s) == 1)
		return  ;
	i = 0;
	while (s[i])
	{
		if (!ft_is_quote(s[i]))
			ft_putchar_fd(s[i], STDOUT_FILENO);
		i++;
	}
}

static void	ft_write_echo_quotes(char *s)
{
	int	i;

	if (!ft_strcmp(s, "\'\'") && ft_strlen(s) == 2)
		return ;
	i = -1;
	while (s[++i] == ' ');
	while (s[i])
	{
		ft_putchar_fd(s[i], STDOUT_FILENO);
		i++;
	}
}

static int	ft_printable_token(t_token *p)
{
	return (!(p->type == INPUT_REDIRECT || p->type == HEREDOC_REDIRECT
			|| p->type == PIPE || p->type == OUTPUT_REDIRECT
			|| p->type == APPEND_REDIRECT));
}

static int	read_flag(t_token **p)
{
	int	nl;

	if (!*p || !(!ft_strcmp((*p)->token, "-n") && ft_strlen((*p)->token) == 2))
		nl = 1;
	else
	{
		while (*p && (!ft_strcmp((*p)->token, "-n")
				&& ft_strlen((*p)->token) == 2))
			*p = (*p)->next;
		if (*p)
			nl = 0;
		else
			nl = 2;
	}
	return (nl);
}

int	ft_echo(t_token *token_list, int status, int is_pipe)
{
	t_token	*p;
	int		nl;

	p = token_list->next;
	nl = read_flag(&p);
	while (p && (ft_printable_token(p) || (!ft_strcmp(p->token, "$?") && ft_strlen(p->token) == 2)))
	{
		if (p->token)
		{
			if (!ft_strcmp(p->token, "$?") && ft_strlen(p->token) == 2)
				ft_putnbr_fd(status, STDOUT_FILENO);
			else if (p->type == DOUBLE_QUOTE || p->type == SINGLE_QUOTE)
				ft_write_echo_quotes(p->token);
			else if (p->next)
				ft_write_echo(p->token);
			else
				ft_write_simple(p->token);
		}
		p = p->next;
		if (p && p->token && p->prev->type != VARIABLE)
			ft_putchar_fd(' ', STDOUT_FILENO);
	}
	if (nl == 0)
		ft_putstr_fd("\x1B[30m\x1B[47m%\x1B[0m\x1B[0m\n", STDOUT_FILENO);
	else if (nl == 1)
		ft_putchar_fd('\n', STDOUT_FILENO);
	if (is_pipe)
		exit (0);
	return (0);
}
