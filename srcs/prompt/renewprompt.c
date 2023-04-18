/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renewprompt.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 10:44:18 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/27 12:37:39 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	renewprompt(int sig)
{
	(void)sig;
	ft_putchar_fd('\n', STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	renewprompt2(int sig)
{
	(void)sig;
	ft_putchar_fd('\n', STDOUT_FILENO);
}
