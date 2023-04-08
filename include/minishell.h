/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 11:41:46 by adpachec          #+#    #+#             */
/*   Updated: 2023/04/04 10:39:38 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define READ_END 0
# define WRITE_END 1

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <errno.h>
# include <sys/wait.h>
# include "../libft/include/libft.h"
# include <sys/wait.h>
# include <string.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>

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
}	t_token;

typedef struct s_pipe
{
	int		i;
	int		num_pipes;
	int		fd[2];
	int		status;
	int		err;
	char	**paths;
	char	**cmd;
	char	*file_path;
	int		fd_in;
	int		fd_out;
	int		num_cmds;
}			t_pipe;

// ******************************* builtins
//	L ft_builtins.c
int		exec_builtins(t_token *token_list, char ***new_environ, int status, int is_pipe);
//	L ft_cd.c
int		ft_cd(t_token *token_list, char **env, int is_pipe);
//	L ft_echo.c
int		ft_echo(t_token *token_list, int status, int is_pipe);
//	L ft_env.c
int		ft_env(char ***new_environ, int is_pipe);
//	L ft_exit.c
int		ft_exit(t_token *token_list, int status, int is_pipe);
//	L ft_export.c
int		ft_export(t_token *token_list, char ***new_environ, int is_pipe);
//	L ft_pwd.c
int		ft_pwd(t_token *token_list, int is_pipe);
//	L ft_unseat.c
int		ft_unset(t_token *token_list, char ***new_environ, int is_pipe);
// ******************************* commands
//	L get_paths_access.c
char	**get_path(char **envp);
int		get_size_cmd(char **cmd);
char	**get_av(char **cmd);
char	*try_access(char **cmd, char **paths);
//	L pipex.c
void	execute_commands(t_token *token_list, char ***new_environ, int *status);
// ******************************* environ
//	L get_environ.c
char	**copy_environ(char **source);
//	L get_vars.c
char	*ft_getenv(char *var_name, char **env);
//	L quotes_expand.c
void	ft_update_double_quote(char **token, char **env);
//	L var_expand.c
void	ft_check_vars(t_token **token_list, char **env);
// ******************************* prompt
//	L prompt.c
void	renewprompt(int signal);
// ******************************* style
//	L splash.c
void	splash(void);
// ******************************* tokenization
//	L ft_is.c
int		ft_is_special(char c);
int		ft_isspace(char c);
int		ft_is_quote(char c);
int 	ft_is_redirect(char c);
int 	ft_is_builtin(char *s);
//	L read_token.c
int		ft_reading_token(char **input);
//	L sort_tokens.c
void	sort_tokens(t_token **token_list);
//	L tokenize_input.c
t_token	*tokenize_input(char *input);
//	L tokenize_utils.c
t_token	*ft_token_last(t_token *lst);
void	ft_token_add_back(t_token **lst, t_token *new);
void	print_token_list(t_token **tokenize_list);
// ******************************* utils
//	L free.c
void	free_matrix(char **matrix);
void	free_environ(char ***e);
void	free_tokens(t_token **token_list);
//	L matrix_utils.c
void	ft_init_matrix(const char *s, char c, char **res, size_t words);
//	L ft_errors.
int		exit_error_token(int err, char *token);
void	error_cmd(int err);
void	exit_error(int err);
#endif