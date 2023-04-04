/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 11:13:20 by adpachec          #+#    #+#             */
/*   Updated: 2023/04/04 10:42:32 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_count_pipes(t_token *token_list)
{
	int		num;
	t_token	*t;

	num = 0;
	t = token_list;
	while (t)
	{
		if (t->type == PIPE)
			++num;
		t = t->next;
	}
	return (num);
}

char	*ft_strjoin_space(char *s1, char *s2)
{
	char	*str;
	size_t	i;
	size_t	len_s1;

	if (!s1 || !s2)
		exit_error(ENOMEM);
	len_s1 = ft_strlen(s1) + 1;
	str = (char *) malloc(sizeof(char) * (len_s1 + ft_strlen(s2) + 2));
	if (!str)
		exit_error(ENOMEM);
	i = -1;
	while (s1[++i])
		str[i] = s1[i];
	str[i] = ' ';
	i = -1;
	while (s2[++i])
		str[len_s1 + i] = s2[i];
	str[len_s1 + i] = '\0';
	free(s1);
	return (str);
}

char	**get_cmd(t_token *token_list, int n_pipes)
{
	t_token	*t;
	char	**cmd;
	int		i;

	cmd = (char	**) ft_calloc(n_pipes + 2, sizeof(char*));	
	t = token_list;
	i = -1;
	while (t)
	{
		if (t->type == COMMAND)
			cmd[++i] = ft_strdup(t->token);
		else if (t->type == ARGUMENT)
			cmd[i] = ft_strjoin_space(cmd[i], t->token);
		t = t->next;
	}
	return (cmd);
}

int	get_infile(t_token *token_list)
{
	t_token	*t;

	t = token_list;
	while (t)
	{
		if (t->type == INPUT_REDIRECT || t->type == HEREDOC_REDIRECT)
			break;
		t = t->next;
	}
	if (t && t->type == INPUT_REDIRECT)
	{
		if (access(t->next->token, F_OK))
		{
			error_cmd(errno);
			return (STDIN_FILENO);
		}
		return (open(t->next->token, O_RDONLY));
	}
	else if (t && t->type == HEREDOC_REDIRECT)
		return (open(t->next->token, O_RDONLY));
	return (STDIN_FILENO);
}

int	get_outfile(t_token *token_list)
{
	t_token	*t;

	t = token_list;
	while (t)
	{
		if (t->type == OUTPUT_REDIRECT || t->type == APPEND_REDIRECT)
			break;
		t = t->next;
	}
	if (t && t->type == OUTPUT_REDIRECT)
		return (open(t->next->token, O_WRONLY | O_CREAT | O_TRUNC, \
		S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH));
	else if (t && t->type == APPEND_REDIRECT)
		return (open(t->next->token, O_WRONLY | O_CREAT | O_APPEND, \
		S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH));
	return (STDOUT_FILENO);
}

int	get_num_cmds(char **cmd)
{
	int	i;

	if (!cmd || !*cmd)
		return (0);
	i = -1;
	while (cmd[++i]);
	return (i);
}

t_pipe	initialize_pipe_struct(t_token *token_list, char **new_environ)
{
    t_pipe  pipe_s;

	pipe_s.i = 0;
	pipe_s.num_pipes = ft_count_pipes(token_list);
	pipe_s.fd_in = get_infile(token_list);
	pipe_s.fd_out = get_outfile(token_list);
	pipe_s.status = 0;
	pipe_s.err = 0;
	pipe_s.paths = get_path(new_environ);
	pipe_s.cmd = get_cmd(token_list, pipe_s.num_pipes);
	pipe_s.num_cmds = get_num_cmds(pipe_s.cmd);
	return (pipe_s);
}

int	exec_command(t_pipe *pipe_s, char **new_environ)
{
	pid_t	pid;
	char	**split_cmd;

	pid = fork();
	if (!pid)
	{
		dup2(pipe_s->fd_in, STDIN_FILENO);
		dup2(pipe_s->fd_out, STDOUT_FILENO);
		split_cmd = ft_split(pipe_s->cmd[pipe_s->i], ' ');
		pipe_s->file_path = try_access(split_cmd, pipe_s->paths);
		// printf("file_path: %s\n", pipe_s->file_path);
		pipe_s->err = execve(pipe_s->file_path, \
		split_cmd, new_environ);
		free_matrix(split_cmd);
		close(pipe_s->fd_in);
		close(pipe_s->fd_out);
		if (pipe_s->err == -1)
			exit(EXIT_FAILURE);
		// printf("\nerr: %d\n", pipe_s->err);
	}
	else
		waitpid(pid, &pipe_s->status, 0);
	return (pipe_s->err);
}

void	pipex(char **new_environ, t_pipe *pipe_s)
{
	int		fd_in;
	pid_t	pid;
	char	**split_cmds;

	fd_in = pipe_s->fd_in;
	pipe_s->i = -1;
	// fprintf(stderr, "ncmds: %d\n", pipe_s->num_cmds);
	while (++pipe_s->i < pipe_s->num_cmds)
	{
		if (pipe(pipe_s->fd) < 0)
		{
			perror("couldn't pipe");
			exit(EXIT_FAILURE);
		}
		pid = fork();
		if (pid == -1)
		{
			perror("couldn't fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			dup2(fd_in, STDIN_FILENO);
			if (pipe_s->i == pipe_s->num_cmds - 1)
				dup2(pipe_s->fd_out, STDOUT_FILENO);
			else
				dup2(pipe_s->fd[WRITE_END], STDOUT_FILENO);
			close(pipe_s->fd[READ_END]);
			close(pipe_s->fd[WRITE_END]);
			// fprintf(stderr, "i: %d\n", pipe_s->i);
			split_cmds = ft_split(pipe_s->cmd[pipe_s->i], ' ');
			pipe_s->file_path = try_access(split_cmds, pipe_s->paths);
			// fprintf(stderr, "fp: %s\n", pipe_s->file_path);
			// int h = -1;
			// char **com = ft_split(pipe_s->cmd[pipe_s->i], ' ');
			// while (com[++h])
			// 	fprintf(stderr, "com: %s\n", com[h]);
			pipe_s->err = execve(pipe_s->file_path, split_cmds, new_environ);
			free_matrix(split_cmds);
			free(pipe_s->file_path);
		}
		else
		{
			waitpid(pid, &pipe_s->status, 0);
			close(pipe_s->fd[WRITE_END]);
			fd_in = pipe_s->fd[READ_END];
		}
	}
}

void	free_pipe(t_pipe *pipe_s)
{
	free_matrix(pipe_s->cmd);
	free_matrix(pipe_s->paths);
}

int 	execute_commands(t_token *token_list, char **new_environ)
{
	t_pipe	pipe_s;
	int		status;

	pipe_s = initialize_pipe_struct(token_list, new_environ);
	if (pipe_s.num_pipes == 0)
		exec_command(&pipe_s, new_environ);
	else
		pipex(new_environ, &pipe_s);
	status = pipe_s.status % 129;
	free_pipe(&pipe_s);
	return (status);
}
