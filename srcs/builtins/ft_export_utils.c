/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 12:03:36 by adpachec          #+#    #+#             */
/*   Updated: 2023/04/19 17:15:46 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	len_token_without_quotes(char *token)
{
	int	i;
	int	len;

	i = -1;
	len = 0;
	while (token[++i])
	{
		if (!ft_is_quote(token[i]))
			++len;
	}
	return (len);
}

char	*ft_eliminate_quotes(char *token)
{
	char		*new_token;
	const int	len = len_token_without_quotes(token);
	int			i;
	int			j;

	if (len > 0)
	{
		new_token = (char *) ft_calloc(len + 1, sizeof(char));
		i = -1;
		j = -1;
		while (token[++i])
		{
			if (!ft_is_quote(token[i]))
				new_token[++j] = token[i];
		}
		return (new_token);
	}
	return (token);
}
