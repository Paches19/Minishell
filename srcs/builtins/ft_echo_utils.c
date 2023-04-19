/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 22:38:49 by jutrera-          #+#    #+#             */
/*   Updated: 2023/04/12 22:38:49 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_write_simple(char *s)
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

void	ft_write_percent(int nl)
{
	if (nl == 1)
		ft_putchar_fd('\n', STDOUT_FILENO);
}

void	ft_write_spaces(t_token *p)
{
	if (p && p->token && !(p->prev->type == VARIABLE \
	&& (!p->prev->token || p->type == DOUBLE_QUOTE)))
		ft_putchar_fd(' ', STDOUT_FILENO);
}
