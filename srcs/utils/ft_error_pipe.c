/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 18:18:03 by jutrera-          #+#    #+#             */
/*   Updated: 2023/05/08 18:44:53 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_error_pipe(void)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
}

int	ft_is_special2(char c)
{
	return (c == '|' || c == '<' || c == '>');
}
