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

static int	ft_count_pipes(t_token *token_list)
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

static char	*ft_strjoin_space(char *s1, char *s2)
{
	char	*str;
	size_t	i;
	size_t	len_s1;

	if (!s1)
		exit_error(ENOMEM);
	if (!s2)
		return (s1);
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

static int	ft_is_redirect_token(t_token *token)
{
	return (token->type == INPUT_REDIRECT || token->type == OUTPUT_REDIRECT \
	|| token->type == APPEND_REDIRECT || token->type == HEREDOC_REDIRECT);
}

static char	**get_cmd(t_token *token_list, int n_pipes)
{
	t_token	*t;
	char	**cmd;
	int		i;

	cmd = (char	**) ft_calloc(n_pipes + 2, sizeof(char*));	
	t = token_list;
	i = -1;
	while (t)
	{
		if (t->type == COMMAND || t->type == BUILTIN)
		{
			cmd[++i] = ft_strdup(t->token);
			t = t->next;
			while (t && t->type != PIPE && !ft_is_redirect_token(t))
			{
				cmd[i] = ft_strjoin_space(cmd[i], t->token);
				t = t->next;
			}
		}
		else
			t = t->next;
	}
	return (cmd);
}

static int	get_infile(t_token *token_list)
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
		if (access(t->token, F_OK))
		{
			error_cmd(errno);
			return (STDIN_FILENO);
		}
		return (open(t->token, O_RDONLY));
	}
	else if (t && t->type == HEREDOC_REDIRECT)
		return (open(t->token, O_RDONLY));
	return (STDIN_FILENO);
}

static int	get_outfile(t_token *token_list)
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
		return (open(t->token, O_WRONLY | O_CREAT | O_TRUNC, \
		S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH));
	else if (t && t->type == APPEND_REDIRECT)
		return (open(t->token, O_WRONLY | O_CREAT | O_APPEND, \
		S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH));
	return (STDOUT_FILENO);
}

static int	get_num_cmds(char **cmd)
{
	int	i;

	if (!cmd || !*cmd)
		return (0);
	i = -1;
	while (cmd[++i]);
	return (i);
}

static t_pipe	initialize_pipe_struct(t_token *token_list, char **new_environ, int *status)
{
    t_pipe  pipe_s;

	pipe_s.i = 0;
	pipe_s.num_pipes = ft_count_pipes(token_list);
	pipe_s.fd_in = get_infile(token_list);
	pipe_s.fd_out = get_outfile(token_list);
	pipe_s.status = *status;
	pipe_s.err = 0;
	pipe_s.paths = get_path(new_environ);
	pipe_s.cmd = get_cmd(token_list, pipe_s.num_pipes);
	pipe_s.num_cmds = get_num_cmds(pipe_s.cmd);
	return (pipe_s);
}

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

static void	exec_command(t_pipe *pipe_s, char ***new_environ)
{
	pid_t	pid;
	char	**split_cmd;
	t_token	*token_list;
	int		stdout_cpy;

	split_cmd = ft_split(pipe_s->cmd[pipe_s->i], ' ');
	if (ft_is_builtin(split_cmd[0]))
	{
		stdout_cpy = dup(STDOUT_FILENO);
		ft_dup_fd(&pipe_s);
		token_list = tokenize_input(pipe_s->cmd[pipe_s->i]);
		//print_token_list(&token_list);
		pipe_s->err = exec_builtins(token_list, new_environ, pipe_s->status, 0);
		free_tokens(&token_list);
		ft_close_out(stdout_cpy);
		pipe_s->status = (unsigned char)pipe_s->err;
	}
	else
	{
		pid = fork();
		if (!pid)
		{
			ft_dup_fd(&pipe_s);
			pipe_s->file_path = try_access(split_cmd, pipe_s->paths);
			pipe_s->err = execve(pipe_s->file_path, split_cmd, *new_environ);
			if (pipe_s->err == -1)	
				exit(EXIT_FAILURE);
		}
		else
		{
			waitpid(pid, &pipe_s->status, 0);
			pipe_s->status %= 129;
		}
	}
	free_matrix(split_cmd);
}

static void	pipex(t_pipe *pipe_s, char ***new_environ)
{
	pid_t	pid;
	char	**split_cmds;
	t_token	*token_list;
	int		fd_in;

	pipe_s->i = -1;
	fd_in = pipe_s->fd_in;
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
			split_cmds = ft_split(pipe_s->cmd[pipe_s->i], ' ');
			if (ft_is_builtin(split_cmds[0]))
			{
				token_list = tokenize_input(pipe_s->cmd[pipe_s->i]);
				pipe_s->err = exec_builtins(token_list, new_environ, pipe_s->status, 1);
				pipe_s->status = (unsigned char)pipe_s->err;
				free_tokens(&token_list);
			}
			else
			{
				pipe_s->file_path = try_access(split_cmds, pipe_s->paths);
				pipe_s->err = execve(pipe_s->file_path, split_cmds, *new_environ);
				pipe_s->status = pipe_s->err % 129;
			}
			free_matrix(split_cmds);
			if (pipe_s->err < 0)
			{
				ft_putstr_fd("err: ", STDERR_FILENO);
				ft_putnbr_fd(pipe_s->err, STDERR_FILENO);
				ft_putchar_fd('\n', STDERR_FILENO);
				exit(pipe_s->err);
			}		
		}
		else
		{
			waitpid(pid, &pipe_s->status, 0);
			close(pipe_s->fd[WRITE_END]);
			fd_in = pipe_s->fd[READ_END];
		}
	}
}

void	execute_commands(t_token *token_list, char ***new_environ, int *status)
{
	t_pipe	pipe_s;

	if (token_list)
	{
		pipe_s = initialize_pipe_struct(token_list, *new_environ, status);
		if (pipe_s.num_pipes == 0)
		{
			exec_command(&pipe_s, new_environ);
			*status = (unsigned char)pipe_s.status;
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
