/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 12:37:43 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/29 12:29:42 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char **argv, char **env)
{
	t_token	*token_list;
	char 	*inpt;
	int		status;
	char	**new_environ;

	if (argc)
		argc = 0;
	if (argv)
		argv = 0;
	atexit(ft_leaks);
	splash();
	signal(SIGINT, &renewprompt);
	signal(SIGQUIT, SIG_IGN);
	status = 0;
	new_environ = copy_environ(env);
	inpt = readline("minishell -> ");
	while (inpt)
	{
		if (*inpt != 0)
			add_history(inpt);
		token_list = tokenize_input(inpt);
		// print_token_list(&token_list);
		ft_check_vars(&token_list, new_environ);
		if (token_list && token_list->type == BUILTIN)
			status = exec_builtins(token_list, &new_environ, status);
		// sort_tokens(&token_list);
		if (token_list && ft_strcmp(token_list->token, "exit") == 0)
			break;
		pipex(token_list, new_environ);
		// if (token_list && token_list->type == COMMAND)
		// 	status = exec_nobuiltins(token_list, new_environ);
		free(inpt);
		print_token_list(&token_list);
		free_tokens(&token_list);
		inpt = readline("minishell -> ");
	}
	free(inpt);
	free_tokens(&token_list);
	free_environ(&new_environ);
	rl_clear_history();
	printf("exit (with status %i)\n", status);
	return (status);
}