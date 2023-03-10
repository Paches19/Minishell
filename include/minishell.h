/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 11:41:46 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/10 12:33:26 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
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