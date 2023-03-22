/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 11:41:46 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/22 10:52:01 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <errno.h>
# include "../libft/libft.h"

typedef enum e_token_type
{
   COMMAND,
   ARGUMENT,
   PIPE,
   INPUT_REDIRECT,
   OUTPUT_REDIRECT,
   APPEND_REDIRECT,
   HEREDOC_REDIRECT,
   DOUBLE_QUOTE,
   SINGLE_QUOTE,
   VARIABLE,
   BUILTIN
}  t_token_type;

typedef struct s_token
{
	char			*token;
	t_token_type	type;
	struct s_token	*next;
	struct s_token	*prev;
}                 t_token;

// typedef struct s_env
// {
// 	char			*var_name;
// 	char			*content;
// 	struct s_env	*next;
// }					t_env;

t_token	*tokenize_input(const char *input);
void	free_tokens(t_token **token_list);
void	print_token_list(t_token **tokenize_list);
void	free_matrix(char **matrix);
// void	free_env(t_env **env);
void	exit_error(int err);

#endif