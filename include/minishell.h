/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 11:41:46 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/27 13:27:48 by adpachec         ###   ########.fr       */
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
# include <sys/wait.h>
# include "../libft/include/libft.h"

typedef enum e_token_type
{
   INPUT_REDIRECT,
   HEREDOC_REDIRECT,
   PIPE,
   OUTPUT_REDIRECT,
   APPEND_REDIRECT,
   COMMAND,
   ARGUMENT,
   DOUBLE_QUOTE,
   SINGLE_QUOTE,
   BUILTIN,
   VARIABLE
}  t_token_type;

typedef struct s_token
{
	char			*token;
	t_token_type	type;
	struct s_token	*next;
	struct s_token	*prev;
}                 t_token;

t_token	*tokenize_input(const char *input);
void	free_tokens(t_token **token_list);
void	print_token_list(t_token **tokenize_list);
void	free_matrix(char **matrix);
void	exit_error(int err);
char	*ft_getenv(char *var_name, char **env);
int		ft_is_special(char const c);
int		ft_isspace(char const c);
int		ft_is_quote(char const c);
int		exec_builtins(t_token *token_list, char ***new_environ, int status);
int		ft_builtins_errors(char e);
int		ft_echo(t_token *token_list);
int		ft_cd(t_token *token_list, char **env);
int		ft_env_in_order(char **new_environ, int len);
int		ft_env(char ***new_environ);
int		ft_exit(t_token *token_list, int status);
int		ft_export(t_token *token_list, char ***new_environ);
int		ft_pwd(void);
int		ft_unset(t_token *token_list, char ***new_environ);
void	ft_check_vars(t_token **token_list, char **env);
void	ft_update_double_quote(char **token, char **env);
char	**copy_environ(char **source);
char	*ft_getenv(char *var_name, char **env);
void	free_environ(char ***e);
void	splash(void);
void	sort_tokens(t_token **token_list);
void	renewprompt(int signal);
void	ft_leaks(void);
t_token	*tokenize_input(const char *input);
int		ft_reading_token(const char **input);
t_token	*ft_token_last(t_token *lst);
void	ft_token_add_back(t_token **lst, t_token *new);
void	print_token_list(t_token **tokenize_list);
int		exit_error_token(int err, char *token);
void	exit_error(int err);

#endif