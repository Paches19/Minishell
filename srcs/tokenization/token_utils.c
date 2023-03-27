/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 13:15:15 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/27 13:20:27 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*ft_convert_type(t_token_type   type)
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
		//printf("\b\btoken: %s\n", aux->token);
		printf("content: %s\n", aux->token);
		printf("\b\btype: %s\n\n", ft_convert_type(aux->type));
		aux = aux->next;
	}
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
