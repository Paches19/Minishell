/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_pipe_struct_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 23:12:58 by jutrera-          #+#    #+#             */
/*   Updated: 2023/04/12 23:12:58 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*ft_last_inheredoc(t_token *token_list)
{
	t_token	*t;
	t_token	*last_redirect;

	t = token_list;
	last_redirect = NULL;
	while (t)
	{
		if (t->type == HEREDOC_REDIRECT || t->type == HEREDOC_QUOTE)
			last_redirect = t;
		t = t->next;
	}
	return (last_redirect);
}

t_token	*ft_last_inredirect(t_token *token_list)
{
	t_token	*t;
	t_token	*last_redirect;

	t = token_list;
	last_redirect = NULL;
	while (t)
	{
		if (t->type == INPUT_REDIRECT)
			last_redirect = t;
		t = t->next;
	}
	return (last_redirect);
}

t_token	*ft_last_outredirect(t_token *token_list)
{
	t_token	*t;
	t_token	*last_redirect;

	t = token_list;
	last_redirect = NULL;
	while (t)
	{
		if (t->type == OUTPUT_REDIRECT || t->type == APPEND_REDIRECT)
			last_redirect = t;
		t = t->next;
	}
	return (last_redirect);
}

int	ft_message_bad_name(char *s)
{
	ft_putstr_fd("minishell: no such file", STDERR_FILENO);
	ft_putstr_fd(" or directory: ", STDERR_FILENO);
	ft_putstr_fd(s, STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
	return (-1);
}

char	*try_pwd_access(char **split_cmd, char ***new_environ, char *file_path)
{
	char	*new_path;
	char	*pwd;

	if (file_path)
		free(file_path);
	if (split_cmd[0][0] == '.')
	{
		pwd = ft_getenv("$PWD", *new_environ);
		new_path = ft_strjoin2(pwd, *split_cmd + 1);
		return (new_path);
	}
	return (NULL);
}
