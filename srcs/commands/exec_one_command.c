/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_one_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:09:08 by jutrera-          #+#    #+#             */
/*   Updated: 2023/04/13 13:46:27 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	ft_dup_fd(t_pipe **pipe_s, int *stdout_cpy, int *stdin_cpy)
{
	*stdout_cpy = dup(STDOUT_FILENO);
	*stdin_cpy = dup(STDIN_FILENO);
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

static void	ft_close_out(int stdout_cpy, int stdin_cpy)
{
	close(STDOUT_FILENO);
	dup2(stdout_cpy, STDOUT_FILENO);
	close(stdout_cpy);
	close(STDIN_FILENO);
	dup2(stdin_cpy, STDIN_FILENO);
	close(stdin_cpy);
}

static void	execute_nobuilting(char **split_cmd, t_pipe *pipe_s,
		char ***new_environ)
{
	int	stdout_cpy;
	int	stdin_cpy;

	signal(SIGQUIT, &renewprompt);
	ft_dup_fd(&pipe_s, &stdout_cpy, &stdin_cpy);
	pipe_s->file_path = try_access(split_cmd, pipe_s->paths, new_environ);
	pipe_s->err = execve(pipe_s->file_path, split_cmd, *new_environ);
	if (pipe_s->err == -1)
	{
		free_matrix(pipe_s->cmd);
		free_matrix(pipe_s->paths);
		exit (EXIT_FAILURE);
	}
}

static unsigned char	execute_builting(t_token *token_list, t_pipe *pipe_s,
		char ***new_environ)
{
	int	stdout_cpy;
	int	stdin_cpy;

	ft_dup_fd(&pipe_s, &stdout_cpy, &stdin_cpy);
	pipe_s->err = exec_builtins(token_list, new_environ, pipe_s->status, 0);
	ft_close_out(stdout_cpy, stdin_cpy);
	return (pipe_s->err);
}

void	exec_one_command(t_token *token_list, t_pipe *pipe_s,
		char ***new_environ)
{
	pid_t	pid;
	char	**split_cmd;

	split_cmd = ft_split(pipe_s->cmd[pipe_s->i], ' ');
	if (split_cmd && ft_is_builtin(split_cmd[0]))
		pipe_s->status = execute_builting(token_list, pipe_s, new_environ);
	else if (split_cmd)
	{
		pid = fork();
		if (!pid)
			execute_nobuilting(split_cmd, pipe_s, new_environ);
		else
		{
			waitpid(pid, &pipe_s->status, 0);
			pipe_s->status %= 129;
			if (pipe_s->status > 0 && pipe_s->status != 2)
				printf("minishell: %s: command not found\n", split_cmd[0]);
			else if (pipe_s->status == 2)
				pipe_s->status = 130;
		}
	}
	free_matrix(split_cmd);
}
