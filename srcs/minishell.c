/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 11:41:29 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/14 17:26:00 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"

void	exit_error(int err)
{
	perror("error\n");
	exit(err);
}

int	exit_error_token(int err, char *token)
{
	char	c;
	
	if (err == -2)
	{
		write(2, "minishell: syntax error near unexpected token `", 47);
		c = *token;
		write(2, &c, 1);
		write(2, "\'\n", 2);
	}
	if (err == -1)
		write(2,"minishell: open quotes. Please close quotes if you use \
		them\n", 65);
	return (-1);
}

void	free_tokens(t_token **token_list) 
{
	t_token *current_node;
	t_token *next_node;

	if (!token_list || !(*token_list))
		return;
	current_node = *token_list;
	while (current_node != NULL) 
	{
		next_node = current_node->next;
		if (current_node->token != NULL)
			free(current_node->token);
		free(current_node);
		current_node = next_node;
	}
	*token_list = NULL;
}

int ft_isspace(char const c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || \
	c == '\v');
}

int ft_is_special(char const c)
{
    return (c == '|' || c == '<' || c == '>' || c == '$');
}

int	check_builtin(const char *token, int len)
{
	if (!ft_strcmp(token, "echo") && len == 4)
		return (BUILTIN);
	else if (!ft_strcmp(token, "cd") && len == 2)
		return (BUILTIN);
	else if (!ft_strcmp(token, "pwd") && len == 3)
		return (BUILTIN);
	else if (!ft_strcmp(token, "export") && len == 6)
		return (BUILTIN);
	else if (!ft_strcmp(token, "unset") && len == 5)
		return (BUILTIN);
	else if (!ft_strcmp(token, "env") && len == 3)
		return (BUILTIN);
	return (COMMAND);
}

enum e_token_type	get_token_type(const char *token, int len)
{
	if (*token == '-')
		return ARGUMENT;
	else if (ft_strcmp(token, "|") == 0)
		return PIPE;
	else if (ft_strcmp(token, "<") == 0)
		return INPUT_REDIRECT;
	else if (ft_strcmp(token, ">") == 0)
		return OUTPUT_REDIRECT;
	else if (ft_strcmp(token, ">>") == 0)
		return APPEND_REDIRECT;
	else if (ft_strcmp(token, "<<") == 0)
		return HEREDOC_REDIRECT;
	else if (*token == '\"' && token[ft_strlen(token) - 1] == '\"')
		return DOUBLE_QUOTE;
	else if (*token == '\'' && token[ft_strlen(token) - 1] == '\'')
		return SINGLE_QUOTE;
	else if (*token == '$')
		return VARIABLE;
	else
		return (check_builtin(token, len));
	return COMMAND;
	//falta introducir built in
}

int	find_closing_quote(const char ***input, char quote)
{
	int	len;

	++**input;
	if (!ft_strchr(**input, quote))
		return (-1);
	len = 1;
	while (**input && ***input != quote)
	{
		++**input;
		++len;
	}
	++**input;
	return (++len);
}

int	ft_read_variable(const char ***input)
{
	int	len;

	++**input;
	len = 1;
	while (**input && !ft_isspace(***input) && !ft_is_special(***input))
	{
		++**input;
		++len;
	}
	++**input;
	return (++len);
}

int	ft_check_special(const char *input)
{
	if (input[-2] == input[-1])
		return(2);
	return(-1);
}
int	read_special_char(const char **input)
{
	int	i;

	i = -1;
	while (!ft_isspace(**input) && ft_is_special(**input) && ++i > -1)
		++(*input);
	if (i > 1)
		return (-2);
	if (i == 0)
		return (1);
	if (i == 1)
		return (ft_check_special(*input));
	return (-1);
}

char	*ft_convert_type(t_token_type   type)
{
	if (type == COMMAND)
		return ("COMMAND");
	else if (type == ARGUMENT)
		return ("ARGUMENT");
	else if (type == PIPE)
		return ("PIPE");
	else if (type == INPUT_REDIRECT)
		return ("INPUT_REDIRECT");
	else if (type == OUTPUT_REDIRECT)
		return ("OUTPUT_REDIRECT");
	else if (type == APPEND_REDIRECT)
		return ("APPEND_REDIRECT");
	else if (type == HEREDOC_REDIRECT)
		return ("HEREDOC_REDIRECT");
	else if (type == DOUBLE_QUOTE)
		return ("DOUBLE_QUOTE");
	else if (type == SINGLE_QUOTE)
		return ("SINGLE_QUOTE");
	else if (type == VARIABLE)
		return ("VARIABLE");
	else if (type == BUILTIN)
		return ("BUILTIN");
	else
		return ("UNKNOWN");
}

void	print_token_list(t_token **tokenize_list)
{
	t_token	*aux;
	int		i;

	printf("print token list\n");
	aux = *tokenize_list;
	i = 0;
	while(aux)
	{
		printf("token %d: \n", ++i);
		printf("\b\btoken: %s\n", aux->token);
		printf("\b\btype: %s\n\n", ft_convert_type(aux->type));
		aux = aux->next;
	}
}

int	ft_reading_token(const char **input)
{
	int	len;

	len = 0;
	// printf("input: %s\n", *input);
	while (**input != '\0')
	{
		// printf("char: %c\n", **input);
		if (**input == '\'' || **input == '\"')
			return (find_closing_quote(&input, **input));
		else if(**input == '$')
			return (ft_read_variable(&input));
		else if(ft_is_special(**input))
		{
			// printf("is special\n");
			return (read_special_char(input));
		}
		else if (!ft_isspace(**input) && !ft_is_special(**input))
		{
			//printf("is not special\n");
			++len;
			++*input;
		}
		else
			return (len);
		if (len == 0)
			return (-1);
	}
	return (len);
}

t_token	*ft_token_last(t_token *lst)
{
	t_token	*aux;

	if (!lst)
		return (NULL);
	aux = lst;
	while (aux->next != NULL)
		aux = aux->next;
	return (aux);
}

void	ft_token_add_back(t_token **lst, t_token *new)
{
	t_token	*ptr;

	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	ptr = ft_token_last(*lst);
	new->prev = ptr;
	ptr->next = new;
}

t_token	*add_token_to_list(t_token **list, char *token, int len)
{
	t_token *new_token;

	if (!token)
		return (NULL);
	new_token = (t_token *)ft_calloc(sizeof(t_token), 1);
	if (!token) 
		exit_error(errno); //error malloc
	new_token->token = ft_substr(token, 0, len);
	new_token->type = get_token_type(new_token->token, len);
	if (*list == NULL)
		*list = new_token;
	else
		ft_token_add_back(list, new_token);
	return (*list);
}

t_token	*tokenize_input(const char *input) 
{
	t_token 		*token_list;
	//t_token			*head_token_list;
	char 			*token;
	int				len;

	token_list = NULL;
	// head_token_list = NULL;
	while (*input)
	{
		while (ft_isspace(*input))
			input++;
		if (*input == '\0')
			exit(1); //error en introduccion de comandos no ha leido nada
		token = (char *) input;
		len = ft_reading_token(&input);
		// printf("len: %d\n", len);
		if (len < 0)
		{
			exit_error_token(len, token);
			free_tokens(&token_list);
			return (NULL);
		}
		add_token_to_list(&token_list, token, len);
		// if (!head_token_list)
		// 	head_token_list = token_list;
	}
	// print_token_list(&token_list);
	return (token_list);
}

// int	main(int argc, char **argv)
// {
// 	t_token	*token_list;
	
// 	if (argc == 1)
// 		return (0);
// 	token_list = tokenize_input(argv[1]);
// 	free_tokens(token_list);
// 	return (0);
// }