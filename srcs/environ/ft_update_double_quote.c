/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_update_double_quote.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 12:43:31 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/24 13:28:30 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	ft_count_words(char *token)
{
	int	words;
	int	i;

	words = 1;
	if (token[0] == '$')
		words = 0;
	i = -1;
	while (token[++i])
	{
		if (token[i] == '$' || ft_is_special(token[i]) || \
		ft_is_space(token[i]) || ft_is_quote(token[i]) || \
		!ft_isalpha(token[i]))
			++words;
	}
	return (words);
}

static char	**ft_split_var(char *token)
{
	int			words;
	int			i;
	int			j;
	const int	token_len = ft_strlen(token);
	char		**matrix;

	words = ft_count_words(token);
	matrix = (char **)ft_calloc(words + 1, sizeof(char *));
	words = 0;
	i = 0;
	while (token[i] && i <= token_len)
	{
		j = 0;
		if (token[i + j] == '$' || ft_is_special(token[i + j])
			|| ft_is_space(token[i + j]) || ft_is_quote(token[i + j])
			|| !ft_isalpha(token[i + j]))
			++j;
		while (token[i + j] && token[i + j] != '$'
			&& !ft_is_special(token[i + j]) && !ft_is_space(token[i + j])
			&& !ft_is_quote(token[i + j]) && ft_isalpha(token[i + j]))
			++j;
		matrix[words++] = ft_substr(token + i, 0, j);
		i += j;
	}
	return (matrix);
}

void	ft_update_double_quote(char **token, char **env)
{
	char	**matrix;
	char	*expand_var;
	int		i;
	char	*dollar;

	dollar = ft_strchr(*token, '$');
	if (!dollar || (dollar && *(dollar + 1) == ' '))
		return ;
	matrix = ft_split_var(*token);
	free(*token);
	*token = NULL;
	i = -1;
	while (matrix[++i])
	{
		if (matrix[i][0] == '$')
		{
			expand_var = ft_getenv(matrix[i], env);
			*token = ft_strjoin2(*token, expand_var);
			free(expand_var);
		}
		else
			*token = ft_strjoin2(*token, matrix[i]);
		free(matrix[i]);
	}
	free(matrix);
}
