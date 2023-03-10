/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 11:41:29 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/10 18:57:45 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"

void	exit_error()
{
	perror("error\n");
	exit(1);
}

void	free_tokens(t_token *token_list) 
{
	t_token *tmp;

	while (token_list) 
	{
		tmp = token_list;
		token_list = token_list->next;
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

int	read_special_char(const char **input)
{
	int	i;

	i = -1;
	while (!ft_isspace(input[0][++i]))
	{
		printf("special: %c\n", input[0][i]);
		/*if (input[0][i] == '>' && (input[0][i + 1] == input[0][i] || input[0][i + 1]) != '<' \
		&& (input[0][i + 2]) != input[0][i] && !ft_is_special(input[0][i + 2])) //ME HE FUMADO EL DOLAR
			return (2);
		else if (input[0][i] == '<' && ((input[0][i + 1]) == input[0][i] || input[0][i + 1] != '>') \
		&& ((input[0][i + 2]) != input[0][i]) && !ft_is_special(input[0][i + 2])) //ME HE FUMADO EL DOLAR
			return (2);
		else if (ft_is_special(input[0][i + 1]))
			return (-1);
		else*/
			return (1);
	}
	return (-1);
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
		printf("\b\btype: %d\n", aux->type);
		aux = aux->next;
	}
}

int	ft_reading_token(const char **input)
{
	int	len;

	len = 0;
	//printf("input: %s\n", *input);
	while (**input != '\0')
	{
	//	printf("char: %c\n", **input);
		if (**input == '\'' || **input == '\"')
			return (find_closing_quote(&input, **input));
		else if(ft_is_special(**input))
		{
			//printf("is special\n");
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

	new_token = (t_token *)ft_calloc(sizeof(t_token), 1);
	if (!token) 
		exit_error(); //error malloc
	new_token->token = ft_substr(token, 0, len);
	new_token->type = get_token_type(new_token->token);
	if (*list == NULL)
			*list = new_token;
	else
		ft_token_add_back(list, new_token);
	return (*list);
}

t_token	*tokenize_input(const char *input) 
{
	t_token 		*token_list;
	t_token			*head_token_list;
	char 			*token;
	int				len;

	token_list = NULL;
	head_token_list = NULL;
	while (*input)
	{
		while (ft_isspace(*input))
			input++;
		if (*input == '\0')
			exit(1); //error en introduccion de comandos no ha leido nada
		token = (char *) input;
		len = ft_reading_token(&input);
		//printf("len: %d\n", len);
		if (len < 0)
			exit_error(); //error en introduccion de comandos comillas abiertas
		add_token_to_list(&token_list, token, len);
		//print_token_list(head_token_list);
		if (!head_token_list)
			head_token_list = token_list;
	}
	print_token_list(&head_token_list);
	return (head_token_list);
}

int	main(int argc, char **argv)
{
	t_token	*token_list;
	
	//comentario nuevo
	if (argc == 1)
		return (0);
	token_list = tokenize_input(argv[1]);
	free_tokens(token_list);
	return (0);
}