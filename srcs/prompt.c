/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 10:44:18 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/22 11:34:42 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_errors(char e)
{
	if (e == 'u')
		printf("unset: not enough arguments\n");
	if (e == 'e')
		printf("export: bad identifier\n");
	return (-1);
}

void	free_environ(char ***e)
{
	int	i;

	i = 0;
	while ((*e)[i])
		free((*e)[i++]);
	free(*e);
}

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

char	*varname(char *s, char **new_environ)
{
	int	i;

	i = 0;
	while (new_environ[i] && ft_strstr(new_environ[i], s + 1) == 0)
		i++;
	if (new_environ[i])
		return(ft_strchr(new_environ[i], '=') + 1);
	return (0);
}

char	*ft_getenv(char *var_name, char **env)
{
	int		i;
	int		j;

	if (!env || !var_name)
		return (NULL);
	i = -1;
	while (env[++i])
	{
		if  (!ft_strncmp(var_name, env[i], ft_strlen(var_name)))
		{
			j = -1;
			while (env[i][++j] && env[i][j] != '=');
			return (ft_substr(env[i], j + 1, ft_strlen(env[i] - j)));
		}
	}
	return (NULL);
}

int ft_echo(t_token *token_list)
{
	t_token	*p;
	int		nl;

	p = token_list->next;
	if (!p || (ft_strcmp(p->token, "-n") != 0))
		nl = 1;
	else
	{
		p = p->next;
		nl = 0;
	}
	while (p)
	{
		ft_putstr_fd(p->token, 1);
		p = p->next;
		if (p)
			write(1, " ", 1);
	}
	if (nl == 0)
		printf("\x1B[30m\x1B[47m%%\x1B[0m\x1B[0m\n");
	else
		write(1, "\n", 1);
	return (0);
}



int ft_cd(t_token *token_list, char **env)
{
	char	*dir;
	t_token	*p;
	int		i;

	p = token_list->next;
	if (!p)
		dir = ft_getenv("HOME", env);
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

int ft_env_in_order(char **new_environ, int len)
{
	int		i;
	int 	j;
	char	*x;
	char	**temp;

	i = -1;
	temp = (char **)ft_calloc(len + 1, sizeof(char *));
	while (new_environ[++i])
		temp[i] = ft_strdup(new_environ[i]);
	i = -1;
	while (temp[++i])
	{
		j = i;
		while (temp[++j])
		{
			if (ft_strcmp(temp[i], temp[j]) > 0)
			{
				x = temp[i];
				temp[i] = temp[j];
				temp[j] = x;
			}
		}
	}
	i = -1;
	while (temp[++i])
		printf("%s\n", temp[i]);
	free_environ(&temp);
	return (0);
}

int ft_env(char ***new_environ)
{
	int	i;

	i = -1;
	while ((*new_environ)[++i])
		printf("%s\n", (*new_environ)[i]);
	return (0);
}

int ft_exit(t_token *token_list, int *status)
{
	t_token	*p;

	p = token_list->next;
	if (!p)
		return (*status);
	*status = ft_atoi(p->token);
	return (*status);
}

int ft_unset(t_token *token_list, char ***new_environ)
{
	t_token		*p;
	int			i;
	char		*temp;
	int			stat;

	p = token_list->next;
	if (!p)
		return (ft_errors('u'));
	stat = 0;
	while (p)
	{
		i = 0;
		while ((*new_environ)[i] && ft_strstr((*new_environ)[i], p->token) == 0)
			i++;
		if ((*new_environ)[i])
		{
			temp = (*new_environ)[i];
			while ((*new_environ)[i])
			{
				(*new_environ)[i] = (*new_environ)[i + 1];
				i++;
			}
			free((*new_environ)[i]);
			free(temp);
		}
		stat++;
		p = p->next;
	}
	return (stat);
}

void	unset_var_names(t_token *token_list, char ***new_environ)
{
	int 	i;
	char	*s;
	t_token	*p;
	char	*temp;

	p = token_list->next;
	while (p)
	{
		i = 0;
		s = ft_strdup(p->token);
		while (s[i] && s[i] != '=')
			i++;
		while (s[i])
		{
			s[i] = 0;
			i++;
		}
		i = 0;
		while ((*new_environ)[i] && ft_strstr((*new_environ)[i], s) == 0)
			i++;
		free(s);
		if ((*new_environ)[i])
		{
			temp = (*new_environ)[i];
			while ((*new_environ)[i])
			{
				(*new_environ)[i] = (*new_environ)[i + 1];
				i++;
			}
			free((*new_environ)[i]);
			free(temp);
		}
		p = p->next;
	}
}

int ft_export(t_token *token_list, char ***new_environ)
{
	t_token		*p;
	int			i;
	int			len;
	char		**extend_env;
	char		*s;
	
	len = 0;
	while ((*new_environ)[len])
		len++;
	p = token_list->next;
	if (!p)
		return (ft_env_in_order(*new_environ, len));
	while (p)
	{
		if (!ft_isalpha(p->token[0]))
			return (ft_errors('e'));
		if (!ft_strchr(p->token, '='))
		{
			s = p->token;
			p->token = ft_strjoin(s, "=''");
			free(s);
		}
		unset_var_names(token_list, new_environ);
		extend_env = (char **)ft_calloc(len + 2, sizeof(char *));
		if (!extend_env)
			return (-1);
		i = -1;
		while ((*new_environ)[++i])
			extend_env[i] = ft_strdup((*new_environ)[i]);
		extend_env[i] = ft_strdup(p->token);
		free_environ(new_environ);
		*new_environ = extend_env;
		p = p->next;
	}
	return (0);
}

int	exec_builtins(t_token *token_list, char ***new_environ, int *status)
{
 	if (ft_strcmp(token_list->token, "echo") == 0)
 		return (ft_echo(token_list));
 	else if (ft_strcmp(token_list->token, "cd") == 0)
 		return (ft_cd(token_list, *new_environ));
 	else if (ft_strcmp(token_list->token, "pwd") == 0)
 		return (ft_pwd());
 	else if (ft_strcmp(token_list->token, "export") == 0)
 		return (ft_export(token_list, new_environ));
 	else if (ft_strcmp(token_list->token, "unset") == 0)
 		return (ft_unset(token_list, new_environ));
 	else if (ft_strcmp(token_list->token, "exit") == 0)
		return (ft_exit(token_list, status));
 	return (ft_env(new_environ));
}

void	ft_leaks(void)
{
	system("leaks -q minishell");
}

char	**copy_environ(char **source)
{
	char	**dest;
	int		len;
	int		i;

	len = 0;
	while (source[len])
		len++;
	dest = (char **)ft_calloc(len + 1, sizeof(char *));
	if (!dest)
		return (0);
	i = 0;
	while (source[i])
	{
		dest[i] = ft_strdup(source[i]);
		i++;
	}
	dest[i] = 0;
	return (dest);
}

void	ft_update_var(char **token, char **env)
{
	char *env_var;

	env_var = ft_getenv(*token + 1, env);
	free(*token);
	if (env_var != NULL)
		*token = ft_strdup(env_var);
	else
		*token = NULL;
}

void	ft_check_vars(t_token **token_list, char **env)
{
	t_token	*aux;

	aux = *token_list;
	while(aux)
	{
		if (aux->type == VARIABLE)
			ft_update_var(&(aux->token), env);
		aux = aux->next;
	}
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
	// atexit(ft_leaks);
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
		ft_check_vars(&token_list, new_environ);
		// print_token_list(&token_list);
		if (token_list && token_list->type == BUILTIN)
			status = exec_builtins(token_list, &new_environ, &status);
		if (token_list && ft_strcmp(token_list->token, "exit") == 0)
			break;
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