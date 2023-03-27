/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 13:24:08 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/27 13:24:54 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_matrix(char **matrix)
{
	int	i;

	if(!matrix || !*(matrix))
		return ;
	i = -1;
	while(matrix[++i])
	{
		free(matrix[i]);
		matrix[i] = NULL;
	}
	free(matrix);
	matrix = NULL;
}

void	free_environ(char ***e)
{
	int	i;

	i = 0;
	while ((*e)[i])
		free((*e)[i++]);
	free(*e);
}
