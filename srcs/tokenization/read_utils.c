/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 16:30:09 by jutrera-          #+#    #+#             */
/*   Updated: 2023/04/13 12:21:22 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	ft_check_special(char *input)
{
	if (input[-2] == input[-1])
		return (2);
	return (-1);
}

int	read_variable(char ***input)
{
	int	len;

	++**input;
	if (***input == '?')
	{
		++**input;
		return (2);
	}
	len = 1;
	while (**input && !ft_is_space(***input)
		&& !ft_is_special(***input) && ft_isalnum(***input))
	{
		++**input;
		++len;
	}
	return (len);
}

int	read_special_char(char **input)
{
	int	i;

	i = -1;
	while (!ft_is_space(**input) && ft_is_special(**input) && ++i > -1)
		++(*input);
	if (i > 1)
		return (-2);
	else if (i == 0)
		return (1);
	else if (i == 1)
		return (ft_check_special(*input));
	return (-1);
}

int	read_redirect(char ***input)
{
	int	len;
	int	i;

	len = 1;
	i = -1;
	while (!ft_is_space(***input) && ft_is_special(***input) && ++i > -1)
		++(**input);
	if (i > 1)
		len = -2;
	else if (i == 0)
		len = 1;
	else if (i == 1)
		len = ft_check_special(**input);
	while (ft_is_space(***input))
		++**input;
	if (len > 0)
	{
		len = 0;
		while (**input && !ft_is_space(***input) && !ft_is_special(***input))
		{
			++**input;
			++len;
		}
	}
	return (len);
}
