/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_nobuiltins.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 21:28:04 by jutrera-          #+#    #+#             */
/*   Updated: 2023/03/25 17:25:46 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_freeee(char **s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		free(s[i]);
		++i;
	}
	free(s);
}

int exec_nobuiltins(t_token *token_list, char **env)
{
	t_token	*p;
	char	**folders;
	char	*path;
	int		status;
	char	**args;
	char	*aux;
	pid_t 	pid;
	pid_t   child_pid;
	int		i;
	int		len;

	path = ft_getenv("$PATH", env);
	if (path)
		folders = ft_split(path, ':');
	if (!path || ! folders)
	{
		perror("path");
		return (-1);
	}
	free(path);
	status = -1;
	p = token_list->next;
	len = ft_strlen(token_list->token);
	while (p)
	{
		len += ft_strlen(p->token);
		p = p->next;
	}
	args = (char **)malloc((len + 1) * sizeof(char *));
	p = token_list;
	i = 0;
	while (p)
	{
		args[i] = ft_strdup(p->token);
		i++;
		p = p->next;
	}
	i = 0;
	while (folders[i] && status != 0)
	{
		aux = ft_strjoin("/", token_list->token);
		free(args[0]);
		args[0] = ft_strjoin(folders[i], aux);
		free(aux);
		pid = fork();  //crea proceso hijo
		if (pid == 0) // entra en proceso hijo
		{
			i = execve(args[0], args, 0);
			if (i != 0)
				exit (-1);
		}	
		else if (pid > 0) // entra en proceso padre
       	{
			child_pid = wait(&status) ; // espera a que termine el proceso hijo
			status = WEXITSTATUS(status);
			//free(args[0]);
			//if (WIFEXITED(status))
            //	printf("Child process %d exited with status %d\n", child_pid, status);
        	//else
          	//	printf("Child process %d terminated abnormally\n", child_pid);
		}
		else 
		{
        	perror("fork"); // Error al crear el proceso hijo
        	exit(EXIT_FAILURE);
    	}
		++i;
	}
	ft_freeee(folders);
	ft_freeee(args);
	if (status != 0)  //no se ha encontrado el comando
		perror("minishell: command not found");
	return (status);
}
