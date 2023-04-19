/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 12:29:42 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/24 13:27:55 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	write_message(char *s)
{
	ft_putstr_fd("minishell: cd:", STDERR_FILENO);
	ft_putstr_fd(s, STDERR_FILENO);
	ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
}

static void	ft_upadte_pwd(char ***new_environ)
{
	int		i;
	char	*pwd;
	char	actual_dir[1024];

	i = ft_check_var_exist("OLDPWD", new_environ);
	pwd = ft_getenv("$PWD", new_environ[0]);
	free((*new_environ)[i]);
	(*new_environ)[i] = ft_strjoin("OLDPWD=", pwd);
	free(pwd);
	i = ft_check_var_exist("PWD", new_environ);
	free((*new_environ)[i]);
	(*new_environ)[i] = ft_strjoin("PWD=", getcwd(actual_dir, \
	sizeof(actual_dir)));
}

static int	execute_cd(t_token *p, char ***env)
{
	char	*dir;

	if (!p)
		dir = ft_getenv("$HOME", *env);
	else
	{
		dir = ft_strtrim(p->token, " ");
		if (!ft_strcmp(dir, "-"))
		{
			free(dir);
			dir = ft_getenv("$OLDPWD", *env);
		}
	}
	if (!dir)
		return (1);
	if (chdir(dir) == -1)
	{
		write_message(p->token);
		free(dir);
		return (1);
	}
	ft_upadte_pwd(env);
	free (dir);
	return (0);
}

int	ft_cd(t_token *token_list, char ***env, int is_pipe)
{
	t_token	*p;
	int		status;

	p = token_list->next;
	status = 0;
	if (p && p->next && p->next->token[0] != '\0')
	{
		ft_putstr_fd("minishell: cd: Too many arguments\n", STDERR_FILENO);
		status = 1;
	}
	else
		status = execute_cd(p, env);
	if (is_pipe)
		exit(status);
	return (status);
}
