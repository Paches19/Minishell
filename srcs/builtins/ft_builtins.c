/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 12:39:04 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/29 10:37:02 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	exec_builtins(t_token *token_list, char ***new_environ, int status)
{
 	if (ft_strcmp(token_list->token, "echo") == 0)
 		return (ft_echo(token_list, status));
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
