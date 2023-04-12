/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_one_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:09:08 by jutrera-          #+#    #+#             */
/*   Updated: 2023/04/12 18:51:04 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	ft_dup_fd(t_pipe **pipe_s)
{
	if (pipe_s[0]->fd_in != 0)
	{
		dup2(pipe_s[0]->fd_in, STDIN_FILENO);
		close(pipe_s[0]->fd_in);
	}
	if (pipe_s[0]->fd_out != 1)
	{
		dup2(pipe_s[0]->fd_out, STDOUT_FILENO);
		close(pipe_s[0]->fd_out);
	}
}

static void	ft_close_out(int stdout_cpy)
{
	close(STDOUT_FILENO);
	dup2(stdout_cpy, STDOUT_FILENO);
	close(stdout_cpy);
}

void	exec_one_command(t_token *token_list, t_pipe *pipe_s,
		char ***new_environ)
{
	pid_t	pid;
	char	**split_cmd;
	int		stdout_cpy;

	split_cmd = ft_split(pipe_s->cmd[pipe_s->i], ' ');
	if (split_cmd && ft_is_builtin(split_cmd[0]))
	{
		stdout_cpy = dup(STDOUT_FILENO);
		ft_dup_fd(&pipe_s);
		pipe_s->err = exec_builtins(token_list, new_environ, pipe_s->status, 0);
		ft_close_out(stdout_cpy);
		pipe_s->status = (unsigned char)pipe_s->err;
	}
	else if (split_cmd)
	{
		pid = fork();
		if (!pid)
		{
			signal(SIGQUIT, &renewprompt);
			ft_dup_fd(&pipe_s);
			pipe_s->file_path = try_access(split_cmd, pipe_s->paths);
			pipe_s->err = execve(pipe_s->file_path, split_cmd, *new_environ);
			if (pipe_s->err == -1)
			{
				free_matrix(pipe_s->cmd);
				free_matrix(pipe_s->paths);
				exit (EXIT_FAILURE);
			}
		}
		else
		{
			waitpid(pid, &pipe_s->status, 0);
			pipe_s->status %= 129;
		}
	}
	free_matrix(split_cmd);
}