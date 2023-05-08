/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_environ.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 12:46:20 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/27 13:24:45 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	update_shell_lvl(char ***dest)
{
	int		shell_lvl;
	int		i;
	char	*new_lvl;

	shell_lvl = ft_atoi(ft_getenv("$SHLVL", *dest));
	if (shell_lvl)
		shell_lvl++;
	else
		shell_lvl = 1;
	i = ft_check_var_exist("SHLVL", dest);
	new_lvl = ft_itoa(shell_lvl);
	free((*dest)[i]);
	(*dest)[i] = ft_strjoin("SHLVL=", new_lvl);
	free(new_lvl);
}

char	**copy_environ(char **source)
{
	char	**dest;
	int		len;
	int		i;

	len = 0;
	while (source[len])
		len++;
	dest = (char **)ft_calloc(len + 1, sizeof(char *));
	if (!dest)
		return (0);
	i = 0;
	while (source[i])
	{
		dest[i] = ft_strdup(source[i]);
		i++;
	}
	dest[i] = 0;
	update_shell_lvl(&dest);
	return (dest);
}
