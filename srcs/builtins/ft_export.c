/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 12:33:05 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/24 20:55:28 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

static void	ft_extend_env(char *token, char ***new_environ, int len)
{
	char	**extend_env;
	int		i;

	extend_env = (char **)ft_calloc(len + 2, sizeof(char *));
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
}

static void	ft_execute_export(char **token, char ***new_environ, int len)
{
	int	i;
	char	*s;
	
	if (!ft_strchr(*token, '='))
	{
		s = ft_strjoin(*token, "=''");
		free(*token);
		*token = s;
	}
	i = ft_check_var_exist(*token, new_environ);
	if (i >= 0)
		ft_replace_var(*token, new_environ, i);
	else
		ft_extend_env(*token, new_environ, len);
	
}

int ft_export(t_token *token_list, char ***new_environ)
{
	t_token	*p;
	int		i;
	int		len;
	
	len = 0;
	while ((*new_environ)[len])
		len++;
	p = token_list->next;
	if (!p)
		return (ft_env_in_order(*new_environ, len));
	while (p && p->type == COMMAND)
	{
		i = -1;
		if (p->token[0] == '=')
			return (ft_builtins_errors('e'));
		while (p->token[++i] && p->token[i] != '=')
		{
			if (!ft_isalpha(p->token[i]))
				return (ft_builtins_errors('e'));
		}
		ft_execute_export(&p->token, new_environ, len);
		p = p->next;
	}
	return (0);
}