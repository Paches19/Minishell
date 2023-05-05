/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 23:43:28 by jutrera-          #+#    #+#             */
/*   Updated: 2023/05/03 23:43:28 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_exit_fail(char *s)
{
	perror(s);
	return (EXIT_FAILURE);
}

void	child_builtin(t_pipe *pipe_s, char ***new_environ)
{
	t_token	*token_list;

	token_list = tokenize_input(pipe_s->cmd[pipe_s->i],
			*new_environ);
	pipe_s->err = exec_builtins(token_list,
			new_environ, pipe_s->status, 1);
	pipe_s->status = (unsigned char)pipe_s->err;
	free_tokens(&token_list);
}
