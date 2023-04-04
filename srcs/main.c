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

int	lots_of_args(t_token *token_list)
{
	int	i;
	t_token	*p;

	i = 0;
	p = token_list->next;
	while (p)
	{
		i++;
		p = p->next;
	}
	if (i > 1)
	{
		perror("exit: Too many arguments");
		return(1);
	}
	return (0);
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
	atexit(ft_leaks);
	splash();
	signal(SIGINT, &renewprompt);  //Ctrl+C
	signal(SIGQUIT, SIG_IGN); //Ctrl-\ ignored
	//Ctrl+D = '\0', so is not a signal !!!
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
		// print_token_list(&token_list);
		if (token_list && token_list->type == BUILTIN)
			status = exec_builtins(token_list, &new_environ, status);
		// // sort_tokens(&token_list);
		else
			execute_commands(token_list, new_environ);
		// printf("pree pipex\n");
		// pipex(token_list, new_environ);
		if (token_list && ft_strcmp(!token_list->token, "exit") && !lots_of_args(token_list))
			break;
		// if (token_list && token_list->type == COMMAND)
		// 	status = exec_nobuiltins(token_list, new_environ);
		free(inpt);
		// print_token_list(&token_list);
		free_tokens(&token_list);
		// printf("pree leer\n");
		inpt = readline("minishell -> ");
	}
	if (inpt)
		free(inpt);
	free_tokens(&token_list);
	free_environ(&new_environ);
	rl_clear_history();
	printf("exit (with status %i)\n", status);
	return ((unsigned char)status);
}
