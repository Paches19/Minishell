/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 13:12:53 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/27 13:18:11 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	ft_check_special(const char *input)
{
	if (input[-2] == input[-1])
		return(2);
	return(-1);
}

static int	find_closing_quote(const char ***input, char quote)
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

static int	ft_read_variable(const char ***input)
{
	int	len;

	++**input;
	len = 1;
	while (**input && !ft_isspace(***input) && !ft_is_special(***input) && ft_isalpha(***input))
	{
		++**input;
		++len;
	}
	return (len);
}

static int	read_special_char(const char **input)
{
	int	i;

	i = -1;
	while (!ft_isspace(**input) && ft_is_special(**input) && ++i > -1)
		++(*input);
	if (i > 1)
		return (-2);
	if (i == 0)
		return (1);
	if (i == 1)
		return (ft_check_special(*input));
	return (-1);
}

int	ft_reading_token(const char **input)
{
	int	len;

	len = 0;
	while (**input != '\0')
	{
		if (**input == '\'' || **input == '\"')
			return (find_closing_quote(&input, **input));
		else if(**input == '$')
			return (ft_read_variable(&input));
		else if(ft_is_special(**input))
			return (read_special_char(input));
		else
		{
			while (!ft_isspace(**input) && !ft_is_special(**input))
			{
				++len;
				++*input;
			}
			return (len);
		}
		if (len == 0)
			return (-1);
	}
	return (len);
}