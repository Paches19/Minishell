/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_vars.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 12:42:50 by adpachec          #+#    #+#             */
/*   Updated: 2023/04/10 18:00:58 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	ft_update_var(char **token, char **env)
{
	char	*env_var;

	if (!ft_strcmp(*token, "$\0") && ft_strlen(*token) == 1)
	{
		free(*token);
		*token = ft_strdup("$");
		return ;
	}
	if (!ft_strcmp(*token, "$?\0") && ft_strlen(*token) == 2)
	{
		free(*token);
		*token = ft_strdup("$?");
		return ;
	}
	env_var = ft_getenv(*token, env);
	free(*token);
	if (env_var != NULL)
		*token = ft_strdup(env_var);
	else
		*token = NULL;
	free(env_var);
}

void	ft_check_vars(t_token **token_list, char **env)
{
	t_token	*aux;
	char	*s;

	aux = *token_list;
	while (aux)
	{
		if (aux->type == VARIABLE || aux->type == DOUBLE_QUOTE
			|| aux->type == SINGLE_QUOTE)
		{
			if (aux->type == DOUBLE_QUOTE || aux->type == SINGLE_QUOTE)
			{
				s = ft_strdup(aux->token + 1);
				s[ft_strlen(s) - 1] = 0;
				free(aux->token);
				aux->token = s;
			}
			if (aux->type == VARIABLE)
				ft_update_var(&(aux->token), env);
			else if (aux->type == DOUBLE_QUOTE)
				ft_update_double_quote(&(aux->token), env);
		}	
		aux = aux->next;
	}
}
