/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 12:39:04 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/25 18:09:43 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	exec_builtins(t_token *token_list, char ***new_environ, int status)
{
 	if (ft_strcmp(token_list->token, "echo") == 0)
 		return (ft_echo(token_list));
 	else if (ft_strcmp(token_list->token, "cd") == 0)
 		return (ft_cd(token_list, *new_environ));
 	else if (ft_strcmp(token_list->token, "pwd") == 0)
 		return (ft_pwd());
 	else if (ft_strcmp(token_list->token, "export") == 0)
 		return (ft_export(token_list, new_environ));
 	else if (ft_strcmp(token_list->token, "unset") == 0)
 		return (ft_unset(token_list, new_environ));
 	else if (ft_strcmp(token_list->token, "exit") == 0)
		return (ft_exit(token_list, status));
 	return (ft_env(new_environ));
}