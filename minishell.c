/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 11:41:29 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/07 14:12:05 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_error()
{
	perror("error\n");
	exit(1);
}

void	free_tokens(t_token *tokens) 
{
	t_token *tmp;

	while (tokens) 
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->token);
		free(tmp);
	}
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

enum e_token_type	get_token_type(const char *token)
{
	if (!token)
		return COMMAND;
	if (strcmp(token, "|") == 0)
		return PIPE;
	else if (strcmp(token, "<") == 0)
		return INPUT_REDIRECT;
	else if (strcmp(token, ">") == 0)
		return OUTPUT_REDIRECT;
	else if (strcmp(token, ">>") == 0)
		return APPEND_REDIRECT;
	else if (strcmp(token, "<<") == 0)
		return HEREDOC_REDIRECT;
	else if (*token == '\"' && token[ft_strlen(token) - 1] == '\"')
		return DOUBLE_QUOTE;
	else if (*token == '\'' && token[ft_strlen(token) - 1] == '\'')
		return SINGLE_QUOTE;
	else if (*token == '$')
		return VARIABLE;
	else
		return ARGUMENT;
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
	return (len);
}

int	read_special_char(const char ***input)
{	
	while (**input)
	{
		if (***input == '>' && (***(input + 1) == ***input || ***(input + 1) != '<') \
		&& (***(input + 2) != ***input && !ft_is_special(***(input + 2)))) //ME HE FUMADO EL DOLAR
		{
			++(**input);
			++(**input);
			return (2);
		}
		
	}
}

int	ft_reading_token(const char **input)
{
	int	len;

	len = 0;
	while (*input)
	{
		if (**input == '\'' || **input == '\"')
			return (find_closing_quote(*input, **input));
		else if(ft_is_special(**input))
			return (read_special_char(*input));
		else if (!ft_isspace(**input) && !ft_is_special(**input))
		{
			++len;
			++input;
		}
		else
			return (len);
		if (len == 0)
				return (-1);
	}
	return (len);
}

t_token*	add_token_to_list(t_token *list, char *token, int len)
{
	t_token *new_token;

	new_token = (t_token *) malloc(sizeof(t_token));
	if (!token) 
		exit_error(); //error malloc
	new_token->token = ft_substr(token, 0, len);
	new_token->type = get_token_type(new_token->token);
	new_token->next = NULL;
	if (list == NULL) 
		return new_token;
	else 
	{
		t_token *current_token = list;
		while (current_token->next != NULL)
			current_token = current_token->next;
	current_token->next = new_token;
	}
	return list;
}

t_token	*tokenize_input(const char *input) 
{
	t_token 		*list_token;
	t_token			*head_list_token;
	char 			*token;
	int				len;

	head_list_token = NULL;
	list_token = NULL;
	while (*input != '\0') 
	{
		while (ft_isspace(*input))
			input++;
		if (*input == '\0')
			return (NULL); //error en introduccion de comandos no ha leido nada
		token = input;
		len = ft_reading_token(*input);
		if (len < 0)
			exit_error(); //error en introduccion de comandos comillas abiertas
		add_token_to_list(list_token, token, len);
		if (!head_list_token)
			head_list_token = list_token;
		list_token->type = get_token_type(token);
	}
	return list_token;
}
