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

int	ft_check_var_exist(char *token, char ***new_environ)
{
	int	i;
	int	j;
	int	len;

	len = 0;
	while (token[len] && token[len] != '=')
			len++;
	i = -1;
	while ((*new_environ)[++i])
	{
		j = 0;
		while ((*new_environ)[i][j] != '=')
			j++;
		if (len == j && !ft_strncmp(token, (*new_environ)[i], j))
			return (i);
	}
	return (-1);
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
	{
		free((*new_environ)[i]);
		s = ft_eliminate_quotes((*p)->token);
		free((*p)->token);
		(*p)->token = s;
		(*new_environ)[i] = ft_strdup((*p)->token);
	}		
}

static void	check_equal(t_token **p)
{
	char	*equal;
	char	*s;

	equal = ft_strchr((*p)->token, '=');
	if (!equal && (*p)->next && *(*p)->next->token == '=')
	{
		s = ft_strjoin((*p)->token, (*p)->next->token);
		*p = (*p)->next;
		free((*p)->token);
		(*p)->token = s;
	}
}

int	ft_export(t_token *token_list, char ***new_environ, int is_pipe)
{
	t_token	*p;
	int		len;
	int		status;

	len = count_vars(new_environ);
	p = token_list->next;
	if (!p)
		status = env_in_order(*new_environ, len);
	else
	{
		status = 0;
		while (p && (p->type == COMMAND || p->type == DOUBLE_QUOTE \
		|| p->type == SINGLE_QUOTE))
		{
			check_equal(&p);
			if (!export_errors(p->token))
				ft_execute_export(&p, new_environ, &len);
			else
				status = 1;
			p = p->next;
		}
	}
	if (is_pipe)
		exit (status);
	return (status);
}
