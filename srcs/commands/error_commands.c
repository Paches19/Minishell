/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 10:48:22 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/30 11:12:47 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	error_cmd(int err)
{
	perror(strerror(err));
	// exit(err);
}

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*ptr;
	size_t			i;

	ptr = (unsigned char *) s;
	i = -1;
	while (++i < n && n > 0)
		ptr[i] = 0;
}

void	*ft_calloc(size_t count, size_t size)
{
	void			*ptr;
	const size_t	len = count * size;

	if (!count)
		return (malloc(0));
	if (len / count != size)
		error_cmd(EDOM);
	ptr = (void *) malloc(len);
	if (!ptr)
		error_cmd(ENOMEM);
	ft_bzero(ptr, len);
	return (ptr);
}