/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 11:41:46 by adpachec          #+#    #+#             */
/*   Updated: 2023/03/31 12:44:36 by adpachec         ###   ########.fr       */
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
}                 t_token;

typedef struct s_pipe
{
	int		i;
	int		num_pipes;
	int		*fd;
	int		status;
	int		err;
	pid_t	pid;
	char	**paths;
	char	**cmd;
	char	*file_path;
	int		fd_in;
	int		fd_out;
	int		num_cmds;
}			t_pipe;

t_token	*tokenize_input(char *input);
void	free_tokens(t_token **token_list);
void	free_matrix(char **matrix);
char	*ft_getenv(char *var_name, char **env);
int		ft_is_special(char c);
int		ft_isspace(char c);
int		ft_is_quote(char c);
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
void	free_environ(char ***e);
void	splash(void);
void	sort_tokens(t_token **token_list);
void	renewprompt(int signal);
void	ft_leaks(void);
int		ft_reading_token(char **input);
t_token	*ft_token_last(t_token *lst);
void	ft_token_add_back(t_token **lst, t_token *new);
void	print_token_list(t_token **tokenize_list);
int		exit_error_token(int err, char *token);
void	exit_error(int err);

void	pipex(char **new_environ, t_pipe *pipe_s);
void 	pipe_exec(char **new_environ, t_pipe *pipe_s, int in_fd);
void	first_son(int fd1[2], t_token *token_list, char **new_environ);
void	second_son(int fd1[2], t_token *token_list, char **new_environ);
void	ft_init_matrix(const char *s, char c, char **res, size_t words);
char	**get_path(char **envp);
int		get_size_cmd(char **cmd);
char	**get_av(char **cmd);
char	*try_access(char **cmd, char **paths);
char	*get_paths_cmd_son_2(char ***paths, char ***cmd, char *const *argv, char **envp);
void	error_cmd(int err);
void	execute_commands(t_token *token_list, char **new_environ);

#endif