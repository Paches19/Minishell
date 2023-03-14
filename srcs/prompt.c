/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 10:44:18 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/14 11:03:59 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"

void	splash(void)
{
	printf("===================================================\n\n");
	printf("\x1b[33m");
	printf(" \u2587     \u2587                                    \u2587   \u2587\n");	
	printf(" \u2587\u2587   \u2587\u2587                 \u2587\u2587\u2587\u2587  \u2587       \u2587\u2587   \u2587   \u2587\n");	
	printf(" \u2587  \u2587  \u2587  \u2587         \u2587   \u2587      \u2587      \u2587  \u2587  \u2587   \u2587\n");	
	printf(" \u2587     \u2587     \u2587 \u2587         \u2587\u2587    \u2587\u2587\u2587    \u2587\u2587\u2587   \u2587   \u2587\n");	
	printf(" \u2587     \u2587  \u2587  \u2587   \u2587  \u2587       \u2587  \u2587   \u2587  \u2587     \u2587   \u2587\n");	
	printf(" \u2587     \u2587  \u2587  \u2587   \u2587  \u2587   \u2587\u2587\u2587\u2587   \u2587   \u2587   \u2587\u2587    \u2587   \u2587");
	printf("\n\n");
	printf("\x1b[0m");
	printf("(by adpachec & jutrera-)\n");	
	printf("===================================================\n\n");
}

void renewprompt(int signal)
{
	(void)signal;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

int ft_echo(t_token *token_list)
{
	t_token	*p;
	char	nl;

	p = token_list->next;
	if (!p || (ft_strcmp(p->token, "-n") != 0))
		nl = '\n';
	else
	{
		p = p->next;
		nl = 0;
	}
	while (p)
	{
		ft_putstr_fd(p->token, 1);
		write(1, " ", 1);
		p = p->next;
	}
	write(1, &nl, 1);
	return (1);
}

int ft_cd(t_token *token_list)
{
	printf("%s\n", token_list->token);
	return (1);
}

int ft_pwd(t_token *token_list)
{
	printf("%s\n", token_list->token);
	return (1);
}

int ft_export(t_token *token_list)
{
	printf("%s\n", token_list->token);
	return (1);
}

int ft_unset(t_token *token_list)
{
	printf("%s\n", token_list->token);
	return (1);
}

int ft_env(t_token *token_list)
{
	printf("%s\n", token_list->token);
	return (1);
}

int	its_builtin_command(t_token *token_list)
{
	char *command;

	command = token_list->token;
	if (ft_strcmp(command, "echo") == 0)
		return (ft_echo(token_list));
	else if (ft_strcmp(command, "cd") == 0)
		return (ft_cd(token_list));
	else if (ft_strcmp(command, "pwd") == 0)
		return (ft_pwd(token_list));
	else if (ft_strcmp(command, "export") == 0)
		return (ft_export(token_list));
	else if (ft_strcmp(command, "unset") == 0)
		return (ft_unset(token_list));
	else if (ft_strcmp(command, "env") == 0)
		return (ft_env(token_list));
	return (1);
}

int	main(void)
{
	t_token	*token_list;
	char *inpt;
		
	splash();
   	// newline if "Ctrl-C"
	signal(SIGINT, &renewprompt);
	// ignore "Ctrl-\"
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		inpt = readline("minishell ->");
		if (inpt == 0 || ft_strcmp(inpt, "exit") == 0) //Ctrl-D pressed or command "exit" typed
			break;
		if (inpt)
		{
			add_history(inpt);
			token_list = tokenize_input(inpt);
		}
		else
			renewprompt(0);
		//builtin commands, look into token_list
		// if (!its_builtin_command(token_list))
		// 	printf("no es comando builtin !\n");
		//devuelve 0 si no es command builtin, 1 si lo es
		free(inpt);
		free_tokens(token_list);
	}
	free(inpt);
	rl_clear_history();
    write(1, "exit\n", 5);
	return (0);
}