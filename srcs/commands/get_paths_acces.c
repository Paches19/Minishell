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
		error_cmd(ENOMEM);
	i = -1;
	while (++i < size_cmd)
	{
		av[i] = (char *)ft_calloc(sizeof (char), ft_strlen(cmd[i]) + 1);
		if (!av[i])
		{
			free_matrix(av);
			free_matrix(cmd);
			error_cmd(ENOMEM);
		}
	}
	i = -1;
	while (cmd[++i])
		av[i] = cmd[i];
	free_matrix(cmd);
	return (av);
}

char	*try_access(char **cmd, char **paths)
{
	char	*file_path;
	int		err;
	int		i;

	err = -1;
	i = -1;
	file_path = NULL;
	// printf("path[0]: %s\n", paths[0]);
	if (cmd[0][0] == '/')  //Nos dan el path
		return(*cmd);
	while (paths[++i] && err < 0)
	{
		// printf("llego: %d\n", i);
		if (file_path)
			free(file_path);
		if (paths[i][ft_strlen(paths[i]) - 1] != '/')
			paths[i] = ft_strjoin2(paths[i], "/");
		file_path = ft_strjoin(paths[i], cmd[0]);
		err = access(file_path, X_OK);
	}
	if (err < 0)
	{
		write(2, cmd[0], ft_strlen(cmd[0]));
		write(2, ": command not found\n", 20);
		free(file_path);
		file_path = NULL;
		//return ("^");
		//exit(127);
	}
	return (file_path);
}

char	*get_paths_cmd_son_2(char ***paths, char ***cmd, char *const *argv, char **envp)
{
	char	*file_path;

	paths[0] = get_path(envp);
	cmd[0] = ft_split(argv[3], ' ');
	file_path = try_access(cmd[0], paths[0]);
	return (file_path);
}
