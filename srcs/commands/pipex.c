/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 10:52:14 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/29 13:39:05 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	first_son(int fd1[2], t_token *token_list, char **new_environ)
{
	char	**paths;
	char	**cmd;
	char	*file_path;
	int		err;
	int		fd_input;

	fd_input = open(token_list->token, O_RDONLY);
	if (fd_input < 0)
		error_cmd(fd_input);
	paths = get_path(new_environ);
	cmd = ft_split(token_list->token, ' ');
	file_path = try_access(cmd, paths);
	//cmd = get_av(cmd);
	dup2(fd_input, STDIN_FILENO);
	close(fd_input);
	close(fd1[READ_END]);
	dup2(fd1[WRITE_END], STDOUT_FILENO);
	err = execve(file_path, cmd, NULL);
	close(fd1[WRITE_END]);
	ft_free_matrix(paths);
	ft_free_matrix(cmd);
	free(file_path);	
	if (err < 0)
		error_cmd(err);
}

void	second_son(int fd1[2], t_token *token_list, char **new_environ)
{
	char	**paths;
	char	**cmd;
	char	*file_path;
	int		err;
	int		fd_exit;

	fd_exit = open(token_list->token, O_CREAT | O_WRONLY);
	if (fd_exit < 0)
		error_cmd(fd_exit);
	file_path = get_paths_cmd_son_2(&paths, &cmd, token_list, new_environ);
	//cmd = get_av(cmd);
	/*if (file_path[0] == '^')
	{
		ft_free_matrix(paths);
		ft_free_matrix(cmd);
		exit(127);	
	}*/
	dup2(fd1[READ_END], STDIN_FILENO);
	close(fd1[READ_END]);
	dup2(fd_exit, STDOUT_FILENO);
	err = execve(file_path, cmd, NULL);
	close (fd_exit);
	ft_free_matrix(paths);
	ft_free_matrix(cmd);
	free(file_path);
	if (err < 0)
		error_cmd(err);
}

int	ft_num_pipes(t_token *token_list)
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
		error_management(ENOMEM);
	len_s1 = ft_strlen(s1);
	str = (char *) malloc(sizeof(char) * (len_s1 + ft_strlen(s2) + 2));
	if (!str)
		error_management(ENOMEM);
	i = -1;
	while (s1[++i])
		str[i] = s1[i];
	str[i] = " ";
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
			cmd[i] = strjoin_space(cmd[i], t->token);
	}
}

t_pipe	initialize_pipe_struct(t_token *token_list, char **new_environ, int n_p)
{
	t_pipe	pipe_s;

	pipe_s.i = -1;
	pipe_s.status = 0;
	pipe_s.err = 0;
	pipe_s.paths = get_path(new_environ);
	pipe_s.cmd = get_cmd(token_list, n_p);
	
}

void	pipe_exec(t_token *token_list, char **new_environ, int n_pipes)
{
	pid_t	pid;
	t_pipe	pipe_s;

	pipe_s = initialize_pipe_struct(token_list, new_environ, n_pipes);
	while (n_pipes-- > 0)
	{
		++pipe_s.i;
		pipe_s.status = 0;
		pipe_s.err = pipe(pipe_s.fd);
		if (pipe_s.err < 0)
			error_cmd(pipe_s.err);
		pid = fork();
		if (!pid)
			first_son(pipe_s.fd[pipe_s.i], token_list, new_environ);
		else
		{
			//waitpid(pid, &status, 0);
			//exit_status(status);
			close(pipe_s.fd[pipe_s.i]);
			pid = fork();
			if (!pid)
				second_son(pipe_s.fd[pipe_s.i], token_list, new_environ);
			else
				close(pipe_s.fd[pipe_s.i]);
		}
		waitpid(pid, &pipe_s.status, 0);
		error_cmd(pipe_s.status);
	}
}

void	pipex(t_token *token_list, char **new_environ)
{
	int		num_pipes;
	int		i;

	num_pipes = ft_count_pipes(token_list);
	pipe_exec(token_list, new_environ, num_pipes);
}
