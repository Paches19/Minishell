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
	HEREDOC_QUOTE,
	PIPE,
	OUTPUT_REDIRECT,
	APPEND_REDIRECT,
	COMMAND,
	ARGUMENT,
	DOUBLE_QUOTE,
	SINGLE_QUOTE,
	BUILTIN,
	VARIABLE
}	t_token_type;

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

// ******************************* builtins ***********************************
int		exec_builtins(t_token *token_list, char ***new_environ,
			int status, int is_pipe);
int		ft_cd(t_token *token_list, char **env, int is_pipe);
int		ft_echo(t_token *token_list, int status, int is_pipe);
int		ft_env(char ***new_environ, int is_pipe);
int		ft_exit(t_token *token_list, int status, int is_pipe);
int		ft_export(t_token *token_list, char ***new_environ, int is_pipe);
int		ft_pwd(t_token *token_list, int is_pipe);
int		ft_unset(t_token *token_list, char ***new_environ, int is_pipe);
//		ft_echo_utils.c
void	ft_write_simple(char *s);
void	ft_write_percent(int nl);
void	ft_write_spaces(t_token *p);
// ******************************* commands ***********************************
void	execute_commands(t_token *token_list, char ***new_environ, int *status);
t_pipe	init_pipe_struct(t_token *token_list,
			char **new_environ, int *status);
void	exec_one_command(t_token *token_list, t_pipe *pipe_s, \
		char ***new_environ);
void	pipex(t_pipe *pipe_s, char ***new_environ);
char	**get_cmd(t_token *token_list, int n_pipes);
int		create_heredoc(t_token *t, char **new_environ);
//		create_heredoc_utils.c
void	handler_ctrl_c(int sig);
void	handler_slash(int sig);
void	ft_message_ctrld(char *s);
void	ft_error_fork(void);
void	eliminate_quotes(char **s);
//		get_paths_access.c
char	**get_path(char **envp);
int		get_size_cmd(char **cmd);
char	**get_av(char **cmd);
char	*try_access(char **cmd, char **paths);
//		init_pipe_struct_utils.c
t_token	*ft_last_inheredoc(t_token *token_list);
t_token	*ft_last_inredirect(t_token *token_list);
t_token	*ft_last_outredirect(t_token *token_list);
int		ft_message_bad_name(char *s);
// ******************************* environ ************************************
char	**copy_environ(char **source);
char	*ft_getenv(char *var_name, char **env);
void	ft_update_double_quote(char **token, char **env);
void	ft_check_vars(t_token **token_list, char **env);
int		count_vars(char ***new_environ);
int		env_in_order(char **new_environ, int len);
// ******************************* prompt *************************************
void	renewprompt(int signal);
void	renewprompt2(int signal);
// ******************************* style **************************************
void	splash(void);
// ******************************* tokenization *******************************
int		ft_reading_token(char **input);
void	sort_tokens(t_token **token_list);
t_token	*tokenize_input(char *input, char **env);
int		check_builtin(const char *token, int len);
//		ft_is.c
int		ft_is_special(char c);
int		ft_is_space(char c);
int		ft_is_quote(char c);
int		ft_is_redirect(char c);
int		ft_is_builtin(char *s);
//		tokenize_utils.c
t_token	*ft_token_last(t_token *lst);
void	ft_token_add_back(t_token **lst, t_token *new);
void	print_token_list(t_token **tokenize_list);
//		read_utils.c
int		read_variable(char ***input);
int		read_special_char(char **input);
int		read_redirect(char ***input);
// ******************************* utils **************************************
void	ft_init_matrix(const char *s, char c, char **res, size_t words);
//		free.c
void	free_matrix(char **matrix);
void	free_environ(char ***e);
void	free_tokens(t_token **token_list);
//		ft_errors.c
int		exit_error_token(int err, char *token);
void	cmd_error(int err);
void	exit_error(int err);
int		export_errors(char *s);
#endif