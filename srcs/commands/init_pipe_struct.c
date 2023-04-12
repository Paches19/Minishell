/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_pipe_struct.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:03:15 by jutrera-          #+#    #+#             */
/*   Updated: 2023/04/12 19:48:29 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	handler_ctrl_c(int sig)
{
	(void)sig;
	exit (0);
}

static int	create_heredoc(char *finish)
{
	int		fd;
	char	*line;
	pid_t	pid;
	int		r;

	pid = fork();
	if (pid == -1)
	{
		perror("couldn't fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		r = 0;
		fd = open("/tmp/heredocXXXXXX", O_RDWR | O_CREAT | O_EXCL,
				S_IRUSR | S_IWUSR);
		if (fd == -1)
		{
			perror("open");
			exit(1);
		}
		signal(SIGINT, &handler_ctrl_c);
		while (1)
		{
			line = readline("heredoc> ");
			if (ft_strcmp(line, finish) == 0)
				break ;
			if (line && *line != '\n')
			{
				write(fd, line, ft_strlen(line));
				write(fd, "\n", 2);
				free (line);
			}
			else if (!line)
			{
				ft_putstr_fd("minishell: warning: here", STDOUT_FILENO);
				ft_putstr_fd("-document delimited by end-of", STDOUT_FILENO);
				ft_putstr_fd("-file at this line (wanted '", STDOUT_FILENO);
				ft_putstr_fd(finish, STDOUT_FILENO);
				ft_putstr_fd("')\n", STDOUT_FILENO);
				break ;
			}
		}
		if (line)
			free (line);
		close(fd);
		exit (1);
	}
	else
		waitpid(pid, &r, 0);
	return (r);
}

t_token	*ft_last_inheredoc(t_token *token_list)
{
	t_token	*t;
	t_token	*last_redirect;

	t = token_list;
	last_redirect = NULL;
	while (t)
	{
		if (t->type == HEREDOC_REDIRECT)
			last_redirect = t;
		t = t->next;
	}
	return (last_redirect);
}

t_token	*ft_last_inredirect(t_token *token_list)
{
	t_token	*t;
	t_token	*last_redirect;

	t = token_list;
	last_redirect = NULL;
	while (t)
	{
		if (t->type == INPUT_REDIRECT)
			last_redirect = t;
		t = t->next;
	}
	return (last_redirect);
}

t_token	*ft_last_outredirect(t_token *token_list)
{
	t_token	*t;
	t_token	*last_redirect;

	t = token_list;
	last_redirect = NULL;
	while (t)
	{
		if (t->type == OUTPUT_REDIRECT || t->type == APPEND_REDIRECT)
			last_redirect = t;
		t = t->next;
	}
	return (last_redirect);
}

static int	get_infile(t_token *token_list)
{
	t_token	*t;
	int		r;

	t = ft_last_inredirect(token_list);
	if (!t)
		t = ft_last_inheredoc(token_list);
	if (t && t->type == INPUT_REDIRECT)
	{
		if (access(t->token, F_OK))
		{
			ft_putstr_fd("minishell: no such file", STDERR_FILENO);
			ft_putstr_fd(" or directory: ", STDERR_FILENO);
			ft_putstr_fd(t->token, STDERR_FILENO);
			ft_putchar_fd('\n', STDERR_FILENO);
			return (-1);
		}
		return (open(t->token, O_RDONLY));
	}
	else if (t && t->type == HEREDOC_REDIRECT)
	{
		r = create_heredoc(t->token);
		if (r)
			return (open("/tmp/heredocXXXXXX", O_RDONLY));
		else
		{
			unlink("/tmp/heredocXXXXXX");
			return (-1);
		}
	}
	return (STDIN_FILENO);
}

static int	get_outfile(t_token *token_list)
{
	t_token	*t;

	t = ft_last_outredirect(token_list);
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
	i = 0;
	while (cmd[i])
		i++;
	return (i);
}

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

t_pipe	init_pipe_struct(t_token *token_list,
	char **new_environ, int *status)
{
	t_pipe	pipe_s;

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
