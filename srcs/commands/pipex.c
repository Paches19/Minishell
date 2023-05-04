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

static void	ft_print_access_error(char *path)
{
	char	**cmd;
	int		i;

	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(": command not found\n", 2);
		return ;
	}
	cmd = ft_split(path, '/');
	i = 0;
	while (cmd[i])
		++i;
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd[i - 1], 2);
	ft_putstr_fd(": command not found\n", 2);
	free_matrix(cmd);
}

static void	prepare_child(t_pipe *pipe_s, int *fd_in, t_fd *fds)
{
	int	j;

	signal(SIGQUIT, &renewprompt);
	dup2(*fd_in, STDIN_FILENO);
	if (pipe_s->i == pipe_s->num_cmds - 1)
		dup2(pipe_s->fd_out, STDOUT_FILENO);
	else
		dup2(fds[pipe_s->i].fd[WRITE_END], STDOUT_FILENO);
	j = -1;
	while (++j < pipe_s->num_cmds - 1)
	{
		close(fds[j].fd[READ_END]);
		close(fds[j].fd[WRITE_END]);
	}
}

static void	the_child(t_pipe *pipe_s, char ***new_environ,
	int fd_in, t_fd *fds)
{
	char	**split_cmds;

	prepare_child(pipe_s, &fd_in, fds);
	split_cmds = ft_split(pipe_s->cmd[pipe_s->i], ' ');
	if (ft_is_builtin(split_cmds[0]))
		child_builtin(pipe_s, new_environ);
	else
	{
		pipe_s->file_path = try_access(split_cmds, pipe_s->paths, new_environ);
		if (access(pipe_s->file_path, X_OK) < 0)
			ft_print_access_error(pipe_s->file_path);
		pipe_s->err = execve(pipe_s->file_path, split_cmds, *new_environ);
		pipe_s->status = pipe_s->err % 129;
	}
	free_matrix(split_cmds);
	if (pipe_s->err < 0)
		exit(pipe_s->err);
}

static void	ft_finish_pipex(t_pipe *pipe_s, t_fd *fds, pid_t *pids)
{
	pipe_s->i = -1;
	while (++pipe_s->i < pipe_s->num_cmds - 1)
	{
		close(fds[pipe_s->i].fd[READ_END]);
		close(fds[pipe_s->i].fd[WRITE_END]);
	}
	signal(SIGCHLD, SIG_IGN);
	wait(&pipe_s->status);
	pipe_s->i = -1;
	while (++pipe_s->i < pipe_s->num_cmds - 1)
	{
		if (pids[pipe_s->i] > 0)
			kill(pids[pipe_s->i], SIGTERM);
	}
	free(pids);
	free(fds);
}

void	pipex(t_pipe *pipe_s, char ***new_environ)
{
	int		fd_in;
	pid_t	*pids;
	t_fd	*fds;

	pids = (pid_t *)ft_calloc(sizeof(pid_t), pipe_s->num_cmds);
	fds = (t_fd *)ft_calloc(sizeof(t_fd), pipe_s->num_cmds - 1);
	fd_in = pipe_s->fd_in;
	if (pipe_s->fd_in < 0 || !pids || !fds)
		exit_error(errno);
	pipe_s->i = -1;
	while (++pipe_s->i < pipe_s->num_cmds - 1)
		pipe(fds[pipe_s->i].fd);
	pipe_s->i = -1;
	while (++pipe_s->i < pipe_s->num_cmds)
	{
		pids[pipe_s->i] = fork();
		if (pids[pipe_s->i] == -1)
			exit(ft_exit_fail("couldn't fork"));
		else if (pids[pipe_s->i] == 0)
			the_child(pipe_s, new_environ, fd_in, fds);
		else
			if (pipe_s->i < pipe_s->num_cmds - 1)
				fd_in = fds[pipe_s->i].fd[READ_END];
	}
	ft_finish_pipex(pipe_s, fds, pids);
}
