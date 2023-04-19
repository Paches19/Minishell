/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_paths_acces.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 10:48:58 by adpachec          #+#    #+#             */
/*   Updated: 2023/04/04 10:33:10 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**get_path(char **envp)
{
	int		i;
	char	*env;
	char	**paths;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5))
		++i;
	if (!envp[i])
		return (NULL);
	env = ft_substr(envp[i], 5, ft_strlen(envp[i]));
	paths = ft_split(env, ':');
	free (env);
	return (paths);
}

int	get_size_cmd(char **cmd)
{
	int	size;

	size = 0;
	while (cmd[size])
		++size;
	return (size);
}

char	**get_av(char **cmd)
{
	char	**av;
	int		size_cmd;
	int		i;

	size_cmd = get_size_cmd(cmd);
	av = (char **)ft_calloc(sizeof(char *), size_cmd + 1);
	if (!av)
		cmd_error(ENOMEM);
	i = -1;
	while (++i < size_cmd)
	{
		av[i] = (char *)ft_calloc(sizeof (char), ft_strlen(cmd[i]) + 1);
		if (!av[i])
		{
			free_matrix(av);
			free_matrix(cmd);
			cmd_error(ENOMEM);
		}
	}
	i = -1;
	while (cmd[++i])
		av[i] = cmd[i];
	free_matrix(cmd);
	return (av);
}

// void	command_not_found(char *cmd, char **path)
// {
// 	ft_putstr_fd(cmd, STDERR_FILENO);
// 	ft_putstr_fd(": command not found\n", STDERR_FILENO);
// 	free(*path);
// 	*path = NULL;
// }

char	*try_access(char **cmd, char **paths)
{
	char	*file_path;
	int		err;
	int		i;

	err = -1;
	i = -1;
	file_path = NULL;
	if (!cmd)
		return (NULL);
	if (*cmd && **cmd == '/')
		return (*cmd);
	while (paths && paths[++i] && err < 0)
	{
		if (file_path)
			free(file_path);
		if (paths[i][ft_strlen(paths[i]) - 1] != '/')
			paths[i] = ft_strjoin2(paths[i], "/");
		file_path = ft_strjoin(paths[i], cmd[0]);
		err = access(file_path, X_OK);
	}
	// if (err < 0)
	// 	command_not_found(cmd[0], &file_path);
	return (file_path);
}
