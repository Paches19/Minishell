/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 13:19:59 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/27 13:20:16 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int ft_isspace(char const c)
{
    return (c < 33);
}

int ft_is_special(char const c)
{
    return (c == '|' || c == '<' || c == '>' || c == '$');
}

int ft_is_quote(char const c)
{
    return (c == '"' || c == '\'');
}