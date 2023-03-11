/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 11:41:46 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/11 18:51:37 by jutrera-         ###   ########.fr       */
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
   VARIABLE
}  t_token_type;

typedef struct s_token
{
	char 				*token;
	t_token_type   type;
	struct s_token *next;
   struct s_token *prev;
}                 t_token;

#endif