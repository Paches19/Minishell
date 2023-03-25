/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_nobuiltins.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 21:28:04 by jutrera-          #+#    #+#             */
/*   Updated: 2023/03/25 10:41:23 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int exec_nobuiltins(t_token *token_list, char **env)
{
	t_token	*p;
	char	**folders;
	char	*path;
	int		status;
	char	*args[3];
	char	*aux;
	pid_t 	pid;
	pid_t   child_pid;

	path = ft_getenv("$PATH", env);
	if (path)
		folders = ft_split(path, ':');
	if (!path || ! folders)
	{
		perror("path");
		return (-1);
	}
	status = -1;
	p = token_list;
	while (*folders && status != 0)
	{
		aux = ft_strjoin("/", p->token);
		args[0] = ft_strjoin(*folders, aux);
		free(aux);
		args[1] = NULL;//aquí pondremos los argumentos del comando
		args[2] = NULL;
		pid = fork();  //crea proceso hijo
		if (pid == 0) // entra en proceso hijo
		{
			printf("Child process initiated\n");
			if (execve(args[0], args, 0) != 0)
				exit (-1);
		}	
		else if (pid > 0) // entra en proceso padre
       	{
			child_pid = wait(&status) ; // espera a que termine el proceso hijo
			if (WIFEXITED(status))
            	printf("Child process %d exited with status %d\n", child_pid, WEXITSTATUS(status));
        	else
          		printf("Child process %d terminated abnormally\n", child_pid);
		}
		else 
		{
        	perror("fork"); // Error al crear el proceso hijo
        	exit(EXIT_FAILURE);
    	}
		free(args[0]);
		free(*folders);
		++folders;
	}
	if (!*folders)  //no se ha encontrado el comando
	{
		printf("minishell: command not found: %s\n", p->token);
		return (-1);
	}
	return (status);
}
