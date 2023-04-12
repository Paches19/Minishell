/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_in_order.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 20:42:33 by jutrera-          #+#    #+#             */
/*   Updated: 2023/04/08 20:42:33 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	print_env_ordered(char **temp)
{
	int	i;

	i = 0;
	while (temp[i])
	{
		ft_putstr_fd(temp[i], STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		i++;
	}
}

static void	swap_strings(char **s1, char **s2)
{
	char	*x;

	x = *s1;
	*s1 = *s2;
	*s2 = x;
}

int	env_in_order(char **new_environ, int len)
{
	int		i;
	int		j;
	char	**temp;

	i = -1;
	temp = (char **)ft_calloc(len + 1, sizeof(char *));
	if (!temp)
		return (0);
	while (new_environ[++i])
		temp[i] = ft_strdup(new_environ[i]);
	i = -1;
	while (temp[++i])
	{
		j = i;
		while (temp[++j])
			if (ft_strcmp(temp[i], temp[j]) > 0)
				swap_strings(&temp[i], &temp[j]);
	}
	print_env_ordered(temp);
	free_environ(&temp);
	return (0);
}
