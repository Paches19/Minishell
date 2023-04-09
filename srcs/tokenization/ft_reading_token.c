/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_reading_token.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 13:12:53 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/28 11:36:22 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	find_closing_quote(char ***input, char quote)
{
	int	len;

	++**input;
	if (!ft_strchr(**input, quote))
		return (-1);
	len = 1;
	while (**input && ***input != quote)
	{
		++**input;
		++len;
	}
	++**input;
	return (++len);
}

static int	the_token(char **p, int len)
{
	while (!ft_is_space(**p) && !ft_is_special(**p))
	{
		++len;
		++*p;
	}
	return (len);
}

int	ft_reading_token(char **input)
{
	int	len;

	len = 0;
	while (**input != '\0')
	{
		while (ft_is_space(**input) && **input != '\0')
			++*input;
		if (**input == '\'' || **input == '\"')
			return (find_closing_quote(&input, **input));
		else if (**input == '$')
			return (read_variable(&input));
		else if (ft_is_redirect(**input))
			return (read_redirect(&input));
		else if (ft_is_special(**input))
			return (read_special_char(input));
		else if (**input != '\0')
			return (the_token(input, len));
	}
	return (len);
}
