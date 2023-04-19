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

static int	write_error(char *s, char *dir)
{
	ft_putstr_fd("minishell: cd:", STDERR_FILENO);
	ft_putstr_fd(s, STDERR_FILENO);
	ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	if (s)
		free(s);
	free(dir);
	return (1);
}

static void	ft_upadte_pwd(char ***new_environ, char *dir)
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
	free(dir);
}

char	*ft_get_dir(char **env, char *dir)
{
	char	*new_dir;

	free(dir);
	new_dir = ft_getenv("$OLDPWD", env);
	return (new_dir);
}

static int	execute_cd(t_token *p, char ***env)
{
	char	*dir;
	char	*token;

	token = NULL;
	if (!p || !p->token)
		dir = ft_getenv("$HOME", *env);
	else
	{
		dir = NULL;
		token = ft_strdup(p->token);
		while (p && p->token)
		{
			dir = ft_strjoin2(dir, p->token);
			p = p->next;
		}
		if (!ft_strcmp(dir, "-"))
			dir = ft_get_dir(*env, dir);
	}
	if (!dir)
		return (1);
	if (chdir(dir) == -1)
		return (write_error(token, dir));
	ft_upadte_pwd(env, dir);
	free(token);
	return (0);
}

int	ft_cd(t_token *token_list, char ***env, int is_pipe)
{
	t_token	*p;
	int		status;

	p = token_list->next;
	status = execute_cd(p, env);
	if (is_pipe)
		exit(status);
	return (status);
}
