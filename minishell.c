/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 11:41:29 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/07 19:22:28 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_error()
{
	perror("error\n");
	exit(1);
}

t_token	*ft_token_last(t_token *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

void	ft_token_add_back(t_token **lst, t_token *new)
{
	t_token	*ptr;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	ptr = ft_token_last(*lst);
	ptr->next = new;
	new->prev = ptr;
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
	if (ft_strcmp(token, "|") == 0)
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
			return (2);
		else if (***input == '<' && (***(input + 1) == ***input || ***(input + 1) != '>') \
		&& (***(input + 2) != ***input && !ft_is_special(***(input + 2)))) //ME HE FUMADO EL DOLAR
			return (2);
		else if (ft_is_special(***(input + 1)))
			return (-1);
		else
			return (1);
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

t_token*	add_token_to_list(t_token **list, char *token, int len)
{
	t_token *new_token;

	new_token = (t_token *)ft_calloc(sizeof(t_token), 1);
	if (!token) 
		exit_error(); //error malloc
	new_token->token = ft_substr(token, 0, len);
	new_token->type = get_token_type(new_token->token);
	if (*list == NULL)
		return new_token;
	else 
		ft_token_add_back(list, new_token);
	return list;
}

t_token	*tokenize_input(const char *input) 
{
	t_token 		*token_list;
	t_token			*head_token_list;
	char 			*token;
	int				len;

	head_token_list = NULL;
	token_list = NULL;
	while (*input)
	{
		while (ft_isspace(*input))
			input++;
		if (*input == '\0')
			return (NULL); //error en introduccion de comandos no ha leido nada
		token = input;
		len = ft_reading_token(*input);
		if (len < 0)
			exit_error(); //error en introduccion de comandos comillas abiertas
		add_token_to_list(&token_list, token, len);
		if (!head_token_list)
			head_token_list = token_list;
	}
	return head_token_list;
}
