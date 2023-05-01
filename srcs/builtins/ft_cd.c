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

static void	ft_add_var(char *token, char ***new_environ, int *len)
{
	char	**extend_env;
	int		i;

	extend_env = (char **)ft_calloc(*len + 2, sizeof(char *));
	if (!extend_env)
		return ;
	i = -1;
	while ((*new_environ)[++i])
	{
		extend_env[i] = ft_strdup((*new_environ)[i]);
		free((*new_environ)[i]);
	}
	extend_env[i] = ft_strdup(token);
	free(*new_environ);
	*new_environ = extend_env;
	++*len;
}

static void	ft_update_pwd(char ***new_environ, char *dir)
{
	int		i;
	char	*pwd;
	char	actual_dir[1024];
	int		len;

	len = count_vars(new_environ);
	if (ft_check_var_exist("OLDPWD", new_environ) == -1)
		ft_add_var("OLDPWD=", new_environ, &len);
	if (ft_check_var_exist("PWD", new_environ) == -1)
		ft_add_var("PWD=", new_environ, &len);
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

static char	*ft_get_dir(char **env, char *dir)
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
		return (cd_error(token, dir));
	ft_update_pwd(env, dir);
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
