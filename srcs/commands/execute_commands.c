/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 11:13:20 by adpachec          #+#    #+#             */
/*   Updated: 2023/04/12 18:45:00 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	ft_finish(t_pipe *p)
{
	free_matrix(p->cmd);
	free_matrix(p->paths);
	unlink("/tmp/heredocXXXXXX");
}

void	execute_commands(t_token *token_list, char ***new_environ, int *status)
{
	t_pipe	pipe_s;

	if (token_list && token_list->token)
	{
		signal(SIGINT, &renewprompt2);
		pipe_s = init_pipe_struct(token_list, *new_environ, status);
		if (pipe_s.fd_in == -1)
			*status = 130;
		else if (pipe_s.num_pipes == 0)
		{
			*status = 1;
			if (pipe_s.fd_in != -1)
			{
				exec_one_command(token_list, &pipe_s, new_environ);
				*status = pipe_s.status;
			}
		}
		else
		{
			pipex(&pipe_s, new_environ);
			*status = pipe_s.status % 129;
		}
		ft_finish(&pipe_s);
	}
}
