/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 11:13:20 by adpachec          #+#    #+#             */
/*   Updated: 2023/04/10 19:02:03 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	execute_commands(t_token *token_list, char ***new_environ, int *status)
{
	t_pipe	pipe_s;

	if (token_list)
	{
		pipe_s = init_pipe_struct(token_list, *new_environ, status);
		if (pipe_s.num_pipes == 0)
		{
			if (pipe_s.fd_in != -1)
			{
				exec_one_command(token_list, &pipe_s, new_environ);
				*status = (unsigned char)pipe_s.status;
			}
			else
				*status = 1;
		}
		else
		{
			pipex(&pipe_s, new_environ);
			*status = pipe_s.status % 129;
		}
		free_matrix(pipe_s.cmd);
		free_matrix(pipe_s.paths);
	}
}
