/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 12:33:05 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/29 10:37:11 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int ft_env_in_order(char **new_environ, int len)
{
	int		i;
	int 	j;
	char	*x;
	char	**temp;

	i = -1;
	temp = (char **)ft_calloc(len + 1, sizeof(char *));
	while (new_environ[++i])
		temp[i] = ft_strdup(new_environ[i]);
	i = -1;
	while (temp[++i])
	{
		j = i;
		while (temp[++j])
		{
			if (ft_strcmp(temp[i], temp[j]) > 0)
			{
				x = temp[i];
				temp[i] = temp[j];
				temp[j] = x;
			}
		}
	}
	i = -1;
	while (temp[++i])
	{
		ft_putstr_fd(temp[i], STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
	}
	free_environ(&temp);
	return (0);
}

static int	ft_check_var_exist(char *token, char ***new_environ)
{
	int	i;
	int	j;
	int	len;

	len = -1;
	while (token[++len] && token[len] != '=');
	i = -1;
	while ((*new_environ)[++i])
	{
		j = -1;
		while ((*new_environ)[i][++j] != '=');
		if (len == j && !ft_strncmp(token, (*new_environ)[i], j))
			return (i);
	}
	return (-1);
}

static void	ft_replace_var(char *token, char ***new_environ, int i)
{
	free((*new_environ)[i]);
	(*new_environ)[i] = ft_strdup(token);
}

static void	ft_extend_env(char *token, char ***new_environ, int *len)
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

static void	ft_execute_export(t_token **p, char ***new_environ, int *len)
{
	int		i;
	char	*s;
	char	*equal;
	
	i = ft_check_var_exist((*p)->token, new_environ);
	equal = ft_strchr((*p)->token, '=');
	if (i < 0)
	{
		if (!equal)
		{
			s = ft_strjoin((*p)->token, "=''");
			free((*p)->token);
			(*p)->token = s;
		}
		ft_extend_env((*p)->token, new_environ, len);
	}
	else if (equal && *(equal + 1) != 0)
		ft_replace_var((*p)->token, new_environ, i);		
}

static int	ft_export_error(void)
{
	ft_putstr_fd("export: bad identifier\n", STDERR_FILENO);
	return (1);
}

int ft_export(t_token *token_list, char ***new_environ, int is_pipe)
{
	t_token	*p;
	int		i;
	int		len;
	int		status;
	int		errors;
	char	*equal;
	char 	*s;
	
	len = 0;
	while ((*new_environ)[len])
		len++;
	p = token_list->next;
	if (!p)
	{
		if (is_pipe)
			exit (ft_env_in_order(*new_environ, len));
		return (ft_env_in_order(*new_environ, len));
	}
	status = 0;
	while (p && p->type == COMMAND)
	{
		equal = ft_strchr(p->token, '=');
		if (!equal && p->next && *p->next->token == '=')
		{
			s = ft_strjoin(p->token, p->next->token);
			p = p->next;
			free(p->token);
			p->token = s;
			equal = ft_strchr(p->token, '=');
		}
		i = -1;
		errors = 0;
		if (p->token[0] == '=')
			errors = ft_export_error();
		else
			while (p->token[++i] && p->token[i] != '=' && errors == 0)
				if (!ft_isalpha(p->token[i]) && p->token[i] != '_')
					errors = ft_export_error();
		if (errors == 0)
			ft_execute_export(&p, new_environ, &len);
		status = status || (errors == 1);
		p = p->next;
	}
	if (is_pipe)
		exit (status);
	return (status);
}
