/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_chatgpt.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 13:33:14 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/30 13:56:03 by adpachec         ###   ########.fr       */
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

	cmd = (char	**) ft_calloc(n_pipes + 1, sizeof(char*));	
	t = token_list;
	i = -1;
	while (t)
	{
		if (t->type == COMMAND )
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
	return (pipe_s);
}


void	pipe_exec(char **cmd, char **new_environ, t_pipe *pipe_s, int i)
{
	pid_t   pid;
	int	 fd_in;
	int	 fd_out;

	pipe_s->i++;
	fd_in = pipe_s->fd[(pipe_s->i - 1) * 2];
	fd_out = pipe_s->fd[pipe_s->i * 2 + 1];
	if (pipe(pipe_s->fd + pipe_s->i * 2) < 0)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		if (pipe_s->i > 0)
		{
			if (dup2(fd_in, STDIN_FILENO) < 0)
			{
				perror("dup2");
				exit(EXIT_FAILURE);
			}
			close(fd_in);
		}

		if (pipe_s->i < pipe_s->num_pipes)
		{
			if (dup2(fd_out, STDOUT_FILENO) < 0)
			{
				perror("dup2");
				exit(EXIT_FAILURE);
			}
			close(fd_out);
		}
		pipe_s->file_path = try_access(pipe_s->cmd, pipe_s->paths, \
		pipe_s->i - 1);
		char **split = ft_split(cmd[i], ' ');
		int j = -1;
		while (split[++j])
			printf("cmd[j]: %s\n", split[j]);
		printf("fp: %s\n", pipe_s->file_path);
		if (execve(pipe_s->file_path, ft_split(cmd[i], ' '), new_environ) < 0)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		if (pipe_s->i > 0)
			close(fd_in);
		if (pipe_s->i < pipe_s->num_pipes)
			close(fd_out);
		waitpid(pid, NULL, 0);
	}
}

void execute_commands(t_token *token_list, char **new_environ)
{
	t_pipe	pipe_s;
	int		i;

	pipe_s = initialize_pipe_struct(token_list, new_environ);
	pipe_s.fd = (int *) malloc(sizeof(int) * 2 * pipe_s.num_pipes);
	if (!pipe_s.fd)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	i = -1;
	while (++i < pipe_s.num_pipes)
		pipe_exec(pipe_s.cmd, new_environ, &pipe_s, i);
	free(pipe_s.fd);
}
