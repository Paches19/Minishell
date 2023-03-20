/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 10:44:18 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/20 13:15:56 by adpachec         ###   ########.fr       */
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

int	check_type_echo(t_token_type type)
{
	return (type == COMMAND || type == ARGUMENT || type == DOUBLE_QUOTE \
	|| type == SINGLE_QUOTE  || type == VARIABLE || type == BUILTIN);
}

t_env	*ft_env_last(t_env *lst)
{
	t_env	*aux;

	if (!lst)
		return (NULL);
	aux = lst;
	while (aux->next != NULL)
		aux = aux->next;
	return (aux);
}

void	ft_env_add_back(t_env **lst, t_env *new)
{
	t_env	*ptr;

	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	ptr = ft_env_last(*lst);
	ptr->next = new;
}

t_env	*add_env_to_list(t_env **env, char *environ)
{
	int		i;
	int		j;
	t_env	*new_env;

	if (!env)
		return (NULL);
	new_env = (t_env *)ft_calloc(sizeof(t_env), 1);
	if (!env) 
		exit_error(errno); //error malloc
	i = -1;
	while(environ[++i] != '=');
	new_env->var_name = ft_substr(environ, 0, i + 1);
	j = -1;
	while(environ[++j]);
	new_env->content = ft_substr(environ, i + 1, j);
	if (*env == NULL)
		*env = new_env;
	else
		ft_env_add_back(env, new_env);
	return (*env);
}

void	print_env_list(t_env **env_list)
{
	t_env	*aux;

	printf("print env list\n");
	aux = *env_list;
	while(aux)
	{
		printf("%s%s\n", aux->var_name, aux->content);
		aux = aux->next;
	}
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
	return (0);
}

int ft_cd(t_token *token_list)
{
	char	*dir;
	t_token	*p;
	int		i;

	p = token_list->next;
	if (!p)
		dir = getenv("HOME");
	else
		dir = p->token;
	i = chdir(dir);
	if (i == -1)
		perror("cd");
	return (i);
}

int ft_pwd(void)
{
	char	dir[1024];

	if (getcwd(dir, sizeof(dir)) == NULL)
	{
		perror("pwd");
		return (1);
	}
	else
	{
		printf("%s\n", dir);
		return (0);
	}	
}

int ft_env(t_env **env)
{
	// extern char **environ;
	// char		**c;

	// c = environ;
	// while (*c)
	// {
	// 	printf("%s\n", *c);
	// 	c++;
	// }
	print_env_list(env);
	return (0);
}
int ft_exit(t_token *token_list)
{
	t_token	*p;
	int		i;

	p = token_list->next;
	if (!p)
		return (0);
	else
		i = ft_atoi(p->token);
	return (i);
}

int ft_export(t_token *token_list, t_env **env)
{
	extern char	**environ;
	t_token		*p;
	int			i;
	int			len;
	char		**new_env;

	p = token_list->next;
	if (!p)
		return (ft_env(env));
	len = 0;
	while (environ[len])
		len++;
	new_env = (char **)ft_calloc(len + 1, sizeof(char *));
	if (!new_env)
		return (-1);
	i = -1;
	while (environ[++i])
		new_env[i] = ft_strdup(environ[i]);
	new_env[i] = ft_strdup(p->token);
	//environ = new_env;
	i = -1;
	while (new_env[++i])
	{
		environ[i] = new_env[i];
		//free(new_env[i]);
	}
	//free(new_env);
	return (0);
}

int ft_unset(t_token *token_list)
{
	extern char **environ;
	t_token		*p;
	int			i;

	p = token_list->next;
	if (!p)
		return (-1);
	i = 0;
	while (environ[i] && ft_strstr(environ[i], p->token) == 0)
		i++;
	if (environ[i])
	{
		//printf("Liberando ... %s\n", environ[i]);
		while (environ[i])
		{
			environ[i] = environ[i + 1];
			i++;
		}
		return (0);
	}
	return (-1);
}

int	exec_builtins(t_token *token_list, t_env **env)
{
 	if (ft_strcmp(token_list->token, "echo") == 0)
 		return (ft_echo(token_list));
 	else if (ft_strcmp(token_list->token, "cd") == 0)
 		return (ft_cd(token_list));
 	else if (ft_strcmp(token_list->token, "pwd") == 0)
 		return (ft_pwd());
 	else if (ft_strcmp(token_list->token, "export") == 0)
 		return (ft_export(token_list, env));
 	else if (ft_strcmp(token_list->token, "unset") == 0)
 		return (ft_unset(token_list));
 	else
	 	return (ft_env(env));
}

// int	its_builtin_command( *token)
// {
// 	if (ft_strcmp(token, "echo") == 0)
// 		return (ft_echo(token_list));
// 	else if (ft_strcmp(token, "cd") == 0)
// 		return (ft_cd(token_list));
// 	else if (ft_strcmp(token, "pwd") == 0)
// 		return (ft_pwd(token_list));
// 	else if (ft_strcmp(token, "export") == 0)
// 		return (ft_export(token_list));
// 	else if (ft_strcmp(token, "unset") == 0)
// 		return (ft_unset(token_list));
// 	else if (ft_strcmp(token, "env") == 0)
// 		return (ft_env(token_list));
// 	return (1);
// }

void	ft_update_var(char **token)
{
	char *env_var;

	env_var = getenv(*token + 1);
	free(*token);
	if (env_var != NULL)
		*token = ft_strdup(env_var);
	else
		*token = NULL;
}

void	ft_check_vars(t_token **token_list)
{
	t_token	*aux;

	aux = *token_list;
	while(aux)
	{
		if (aux->type == VARIABLE)
			ft_update_var(&(aux->token));
		aux = aux->next;
	}
}

void	ft_leaks(void)
{
	system("leaks -q minishell");
}

t_env	*ft_get_env(void)
{
	int			len;
	extern char	**environ;
	t_env		*env;

	env = NULL;
	len = -1;
	while(environ[++len])
		env = add_env_to_list(&env, environ[len]);
	return (env);
}

int	main(void)
{
	t_token	*token_list;
	char 	*inpt;
	int		status;
	int		s_exit;
	t_env	*env;

	atexit(ft_leaks);
	env = ft_get_env();
	token_list = NULL;
	splash();
	signal(SIGINT, &renewprompt);
	signal(SIGQUIT, SIG_IGN);
	status = 0;
	inpt = readline("minishell -> ");
	while (inpt)
	{
		if (*inpt != 0)
			add_history(inpt);
		token_list = tokenize_input(inpt);
		if (ft_strcmp(token_list->token, "exit") == 0)
		{
			s_exit = ft_exit(token_list);
			if (s_exit != 0)
				status = s_exit;
			break;
		}
		ft_check_vars(&token_list);
		if (token_list->type == BUILTIN)
			status = exec_builtins(token_list, &env);
		print_token_list(&token_list);
		free_tokens(&token_list);
		free(inpt);
		inpt = readline("minishell -> ");
	}
	free_env(&env);
	free(inpt);
	free_tokens(&token_list);
	rl_clear_history();
    //write(1, "exit\n", 5);
	printf("exit (with status %i)\n", status);
	return (status);
}
