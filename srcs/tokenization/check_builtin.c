/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 19:17:48 by jutrera-          #+#    #+#             */
/*   Updated: 2023/04/13 19:17:48 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_builtin(const char *token, int len)
{
	if (!ft_strcmp(token, "echo") && len == 4)
		return (BUILTIN);
	else if (!ft_strcmp(token, "cd") && len == 2)
		return (BUILTIN);
	else if (!ft_strcmp(token, "pwd") && len == 3)
		return (BUILTIN);
	else if (!ft_strcmp(token, "export") && len == 6)
		return (BUILTIN);
	else if (!ft_strcmp(token, "unset") && len == 5)
		return (BUILTIN);
	else if (!ft_strcmp(token, "env") && len == 3)
		return (BUILTIN);
	else if (!ft_strcmp(token, "exit") && len == 4)
		return (BUILTIN);
	return (COMMAND);
}
