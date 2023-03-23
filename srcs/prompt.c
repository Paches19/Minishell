/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 10:44:18 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/23 17:48:11 by adpachec         ###   ########.fr       */
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

int	ft_printable_token(t_token *p)
{
	return (!(p->type == INPUT_REDIRECT || p->type == HEREDOC_REDIRECT || \
	p->type == PIPE ||p->type == OUTPUT_REDIRECT ||p->type == APPEND_REDIRECT));
}

int ft_echo(t_token *token_list)
{
	t_token	*p;
	int		nl;

	p = token_list->next;
	if (!p || !(!ft_strcmp(p->token, "-n") && ft_strlen(p->token) == 2))
		nl = 1;
	else
	{
		while ((!ft_strcmp(p->token, "-n") && ft_strlen(p->token) == 2))
			p = p->next;
		nl = 0;
	}
	while (p && ft_printable_token(p))
	{
		ft_putstr_fd(p->token, 1);
		p = p->next;
		if (p && p->token)
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

int	ft_delete_var(t_token *p, char ***new_environ)
{
	int		stat;
	int		i;
	char	*temp;

	stat = 0;
	while (p)
	{
		i = -1;
		while ((*new_environ)[++i] && ft_strcmp((*new_environ)[i], p->token));
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

int ft_unset(t_token *token_list, char ***new_environ)
{
	t_token	*p;
	int		stat;

	p = token_list->next;
	stat = 0;
	while (p && p->type == COMMAND)
	{
		if (!p)
		return (ft_errors('u'));
		if (ft_strchr(p->token, '='))
		return (ft_errors('e'));
		stat = ft_delete_var(p, new_environ);
		p = p->next;
	}
	return (stat);
}

int	ft_check_var_exist(char *token, char ***new_environ)
{
	int	i;
	int	j;
	int	len;

	len = -1;
	while (token[++len] && token[len] != '=');
	i = -1;
	while ((*new_environ)[++i])
	{
		j = -1;
		while ((*new_environ)[i][++j] != '=');
		if (len == j && !ft_strncmp(token, (*new_environ)[i], j))
			return (i);
	}
	return (-1);
}

void	ft_replace_var(char *token, char ***new_environ, int i)
{
	free((*new_environ)[i]);
	(*new_environ)[i] = ft_strdup(token);
}

void	ft_extend_env(char *token, char ***new_environ, int len)
{
	char	**extend_env;
	int		i;

	extend_env = (char **)ft_calloc(len + 2, sizeof(char *));
	if (!extend_env)
		return ;
	i = -1;
	while ((*new_environ)[++i])
		extend_env[i] = ft_strdup((*new_environ)[i]);
	extend_env[i] = ft_strdup(token);
	free_environ(new_environ);
	*new_environ = extend_env;
}

int ft_export(t_token *token_list, char ***new_environ)
{
	t_token	*p;
	int		i;
	int		len;
	
	len = 0;
	while ((*new_environ)[len])
		len++;
	p = token_list->next;
	if (!p)
		return (ft_env_in_order(*new_environ, len));
	while (p && p->type == COMMAND)
	{
		if (!ft_isalpha(p->token[0]))
			return (ft_errors('e'));
		if (!ft_strchr(p->token, '='))
			p->token = ft_strjoin(p->token, "=''");
		i = ft_check_var_exist(p->token, new_environ);
		if (i >= 0)
			ft_replace_var(p->token, new_environ, i);
		else
			ft_extend_env(p->token, new_environ, len);
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

char	*ft_getenv(char *var_name, char **env)
{
	int			i;
	int			j;
	const int	var_len = ft_strlen(++var_name);

	if (!env || !var_name)
		return (NULL);
	i = -1;
	while (env[++i])
	{
		if  (!ft_strncmp(var_name, env[i], var_len))
		{
			j = -1;
			while (env[i][++j] && env[i][j] != '=');
			if (var_len == j)
				return (ft_substr(env[i], j + 1, ft_strlen(env[i] - j + 1)));
		}
	}
	return (NULL);
}

void	ft_update_var(char **token, char **env)
{
	char *env_var;

	env_var = ft_getenv(*token, env);
	free(*token);
	if (env_var != NULL)
		*token = ft_strdup(env_var);
	else
		*token = NULL;
	free(env_var);
}

int	count_vars(char *token)
{
	int	vars;
	int	i;

	vars = 1;
	if (token[0] == '$')
		vars = 0;
	i = -1;
	while(token[++i] != '\0')
	{
		if (token[i] == '$')
			++vars;
	}
	return (vars);
}

char	**ft_split_var(char *token)
{
	int			vars;
	int			i;
	int			j;
	const int	token_len = ft_strlen(token);
	char		**matrix;

	vars = count_vars(token);
	matrix = (char **)ft_calloc(vars + 1, sizeof(char *));
	vars = 0;
	i = 0;
	j = 0;
	while (token[i] && vars <= token_len)
	{
		j = 0;
		if (token[i + j] == '$')
			++j;
		while (token[i + j] && token[i + j] != '$')
			++j;
		matrix[vars] = ft_substr(token + i, 0, j);
		++vars;
		i += j;
	}
	return (matrix);
}

void	ft_update_double_quote(char **token, char **env)
{
	char	**matrix;
	char	*expand_var;
	int		i;

	if (!ft_strchr(*token, '$'))
		return ;
	matrix = ft_split_var(*token);
	free(*token);
	*token = NULL;
	i = -1;
	while (matrix[++i])
	{
		if (matrix[i][0] == '$')
		{
			expand_var = ft_getenv(matrix[i], env);
			*token = ft_strjoin2(*token, expand_var);	
			free(expand_var);
		}
		else
			*token = ft_strjoin2(*token, matrix[i]);
		free(matrix[i]);
	}
	free(matrix);
}

void	ft_check_vars(t_token **token_list, char **env)
{
	t_token	*aux;
	char	*s;

	aux = *token_list;
	while(aux)
	{
		if (aux->type == VARIABLE || aux->type == DOUBLE_QUOTE || aux->type == SINGLE_QUOTE)
		{
			if (aux->type == DOUBLE_QUOTE || aux->type == SINGLE_QUOTE)
			{
				s = ft_strdup(aux->token + 1);
				s[ft_strlen(s) - 1] = 0;
				free(aux->token);
				aux->token = s;
			}
			if ((aux->type == VARIABLE))
				ft_update_var(&(aux->token), env);
			else if (aux->type == DOUBLE_QUOTE)
				ft_update_double_quote(&(aux->token), env);
		}	
		aux = aux->next;
	}
}

void swap_tokens(t_token **token_list, t_token *t1, t_token *t2) 
{
    if (t1->prev) 
        t1->prev->next = t2;
    else 
        *token_list = t2;
    if (t2->next)
        t2->next->prev = t1;
    t1->next = t2->next;
    t2->prev = t1->prev;
    t1->prev = t2;
    t2->next = t1;
}

void sort_tokens(t_token **token_list) 
{
    t_token *current;
    int 	swap_occurred;

	current = *token_list;
	swap_occurred = 1;
    while (swap_occurred) 
	{
        swap_occurred = 0;
        while (current->next) 
		{
            if (current->type > current->next->type) 
			{
                swap_tokens(token_list, current, current->next);
                current = current->prev;
                swap_occurred = 1;
            }
            current = current->next;
        }
        while (current->prev)
            current = current->prev;
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
		print_token_list(&token_list);
		// sort_tokens(&token_list);
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