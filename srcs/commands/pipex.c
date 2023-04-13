/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:15:11 by jutrera-          #+#    #+#             */
/*   Updated: 2023/04/09 17:15:11 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	ft_exit_fail(char *s)
{
	perror(s);
	return (EXIT_FAILURE);
}

static int	ft_error_message(int p_err)
{
	ft_putstr_fd("err: ", STDERR_FILENO);
	ft_putnbr_fd(p_err, STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
	exit(p_err);
}

static void	child_builtin(t_pipe *pipe_s, char ***new_environ)
{
	t_token	*token_list;

	token_list = tokenize_input(pipe_s->cmd[pipe_s->i],
			*new_environ);
	pipe_s->err = exec_builtins(token_list,
			new_environ, pipe_s->status, 1);
	pipe_s->status = (unsigned char)pipe_s->err;
	free_tokens(&token_list);
}

static void	the_child(t_pipe *pipe_s, char ***new_environ, int fd_in)
{
	char	**split_cmds;

	signal(SIGQUIT, &renewprompt);
	dup2(fd_in, STDIN_FILENO);
	if (pipe_s->i == pipe_s->num_cmds - 1)
		dup2(pipe_s->fd_out, STDOUT_FILENO);
	else
		dup2(pipe_s->fd[WRITE_END], STDOUT_FILENO);
	close(pipe_s->fd[READ_END]);
	close(pipe_s->fd[WRITE_END]);
	split_cmds = ft_split(pipe_s->cmd[pipe_s->i], ' ');
	if (ft_is_builtin(split_cmds[0]))
		child_builtin(pipe_s, new_environ);
	else
	{
		pipe_s->file_path = try_access(split_cmds, pipe_s->paths);
		pipe_s->err = execve(pipe_s->file_path,
				split_cmds, *new_environ);
		pipe_s->status = pipe_s->err % 129;
	}
	free_matrix(split_cmds);
	if (pipe_s->err < 0)
		exit(ft_error_message(pipe_s->err));
}

void	pipex(t_pipe *pipe_s, char ***new_environ)
{
	pid_t	pid;
	int		fd_in;

	pipe_s->i = -1;
	fd_in = pipe_s->fd_in;
	if (pipe_s->fd_in < 0)
		exit_error(errno);
	while (++pipe_s->i < pipe_s->num_cmds)
	{
		if (pipe(pipe_s->fd) < 0)
			exit(ft_exit_fail("couldn't pipe"));
		pid = fork();
		if (pid == -1)
			exit(ft_exit_fail("couldn't fork"));
		else if (pid == 0)
			the_child(pipe_s, new_environ, fd_in);
		else
		{
			waitpid(pid, &pipe_s->status, 0);
			close(pipe_s->fd[WRITE_END]);
			fd_in = pipe_s->fd[READ_END];
		}
	}
}
