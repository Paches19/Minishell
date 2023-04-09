/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_pipe_struct.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:03:15 by jutrera-          #+#    #+#             */
/*   Updated: 2023/04/09 17:03:15 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	get_infile(t_token *token_list)
{
	t_token	*t;

	t = token_list;
	while (t)
	{
		if (t->type == INPUT_REDIRECT || t->type == HEREDOC_REDIRECT)
			break ;
		t = t->next;
	}
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
			break ;
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
