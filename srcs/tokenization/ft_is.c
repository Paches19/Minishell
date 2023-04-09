/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 13:19:59 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/28 11:29:14 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_is_space(char c)
{
	return (c < 33);
}

int	ft_is_special(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '$');
}

int	ft_is_quote(char c)
{
	return (c == '"' || c == '\'');
}

int	ft_is_redirect(char c)
{
	return (c == '<' || c == '>');
}

int	ft_is_builtin(char *s)
{
	return ((ft_strcmp(s, "echo") == 0)
		|| (ft_strcmp(s, "cd") == 0)
		|| (ft_strcmp(s, "pwd") == 0)
		|| (ft_strcmp(s, "export") == 0)
		|| (ft_strcmp(s, "unset") == 0)
		|| (ft_strcmp(s, "exit") == 0)
		|| (ft_strcmp(s, "env") == 0));
}
