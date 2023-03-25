/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 10:44:18 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/25 18:15:01 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void renewprompt(int signal)
{
	(void)signal;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	ft_leaks(void)
{
	system("leaks -q minishell");
}

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
	//atexit(ft_leaks);
	token_list = NULL;
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
		//print_token_list(&token_list);
		ft_check_vars(&token_list, new_environ);
		
		// sort_tokens(&token_list);
		if (token_list && token_list->type == BUILTIN)
			status = exec_builtins(token_list, &new_environ, status);
		if (token_list && ft_strcmp(token_list->token, "exit") == 0)
			break;
		if (token_list && token_list->type == COMMAND)
			status = exec_nobuiltins(token_list, new_environ);
		free(inpt);
		//print_token_list(&token_list);
		free_tokens(&token_list);
		inpt = readline("minishell -> ");
	}
	free(inpt);
	free_tokens(&token_list);
	free_environ(&new_environ);
	rl_clear_history();
    //write(1, "exit\n", 5);
	printf("exit (with status %i)\n", status);
	return (status);
}