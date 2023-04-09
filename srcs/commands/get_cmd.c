/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:17:23 by jutrera-          #+#    #+#             */
/*   Updated: 2023/04/09 17:17:23 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*ft_strjoin_space(char *s1, char *s2)
{
	char	*str;
	size_t	i;
	size_t	len_s1;

	if (!s1)
		exit_error(ENOMEM);
	if (!s2)
		return (s1);
	len_s1 = ft_strlen(s1) + 1;
	str = (char *) malloc(sizeof(char) * (len_s1 + ft_strlen(s2) + 2));
	if (!str)
		exit_error(ENOMEM);
	i = -1;
	while (s1[++i])
		str[i] = s1[i];
	str[i] = ' ';
	i = -1;
	while (s2[++i])
		str[len_s1 + i] = s2[i];
	str[len_s1 + i] = '\0';
	free(s1);
	return (str);
}

static int	ft_is_redirect_token(t_token *token)
{
	return (token->type == INPUT_REDIRECT || token->type == OUTPUT_REDIRECT \
	|| token->type == APPEND_REDIRECT || token->type == HEREDOC_REDIRECT);
}

char	**get_cmd(t_token *token_list, int n_pipes)
{
	t_token	*t;
	char	**cmd;
	int		i;

	cmd = (char **)ft_calloc(n_pipes + 2, sizeof(char *));
	t = token_list;
	i = -1;
	while (t)
	{
		if (t->type == COMMAND || t->type == BUILTIN)
		{
			cmd[++i] = ft_strdup(t->token);
			t = t->next;
			while (t && t->type != PIPE && !ft_is_redirect_token(t))
			{
				cmd[i] = ft_strjoin_space(cmd[i], t->token);
				t = t->next;
			}
		}
		else
			t = t->next;
	}
	return (cmd);
}
