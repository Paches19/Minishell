/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 12:37:43 by adpachec          #+#    #+#             */
/*   Updated: 2023/04/04 10:45:07 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	clean_memory(char **i, t_token **t, char ***n, int end)
{
	if (i && *i)
		free(*i);
	free_tokens(t);
	if (end)
	{
		free_environ(n);
		rl_clear_history();
	}
}

static void	init_minishell(int *s, char ***n, t_token **t, char **env)
{
	unlink("/tmp/heredocXXXXXX");
	splash();
	signal(SIGINT, &renewprompt);
	signal(SIGQUIT, SIG_IGN);
	*s = 0;
	*n = copy_environ(env);
	*t = NULL;
}

static int	typed_exit(t_token *t)
{
	int		i;
	t_token	*p;

	i = 0;
	p = t->next;
	while (p)
	{
		i++;
		p = p->next;
	}
	if (i > 1)
		return (0);
	return (t && !ft_strcmp(t->token, "exit"));
}

int	main(int argc, char **argv, char **env)
{
	t_token	*token_list;
	char	*input;
	int		status;
	char	**new_environ;

	(void)argc;
	(void )**argv;
	init_minishell(&status, &new_environ, &token_list, env);
	while (1)
	{
		input = readline("\x1b[33mminishell ->\x1b[0m ");
		if (input && *input != 0)
		{
			token_list = tokenize_input(input);
			ft_check_vars(&token_list, new_environ);
			execute_commands(token_list, &new_environ, &status);
			if (token_list && typed_exit(token_list))
				break ;
		}
		else if (!input)
		{
			ft_putchar_fd('\n', STDOUT_FILENO);
			break ;
		}
		clean_memory(&input, &token_list, &new_environ, 0);
	}
	clean_memory(&input, &token_list, &new_environ, 1);
	return ((unsigned char)status);
}
