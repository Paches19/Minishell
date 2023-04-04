/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_errors.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 12:53:17 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/24 13:27:28 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_builtins_errors(char e)
{
	if (e == 'u')
		printf("unset: not enough arguments\n");
	if (e == 'e')
		printf("export: bad identifier\n");
	return (1);
}