/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 23:15:32 by jutrera-          #+#    #+#             */
/*   Updated: 2023/04/12 23:15:32 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	ft_init_child(void)
{
	int	fd;

	fd = open("/tmp/heredocXXXXXX", O_RDWR | O_CREAT | O_EXCL,
			S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		perror("open");
		exit(1);
	}
	signal(SIGINT, &handler_ctrl_c);
	return (fd);
}

static int	ft_putvar_fd(char *s, int i, int fd, char **new_environ)
{
	char	*variable;
	int		len;
	char	*content;

	len = 0;
	while (ft_isalnum(s[len + i]) || s[len + i] == '_' || s[len + i] == '$')
		len++;
	variable = ft_substr(s, i, len);
	content = ft_getenv(variable, new_environ);
	ft_putstr_fd(content, fd);
	free(variable);
	free(content);
	return (len - 1);
}

static void	ft_write_heredoc(int fd, char *line, char **new_environ, t_token *t)
{
	int		i;

	if (t->type == HEREDOC_QUOTE)
		ft_putstr_fd(line, fd);
	else
	{
		i = -1;
		while (line[++i])
		{
			if (line[i] != '$')
				ft_putchar_fd(line[i], fd);
			else
				i += ft_putvar_fd(line, i, fd, new_environ);
		}
	}
}

static int	control_break(int fd, t_token *t, char **new_environ)
{
	char	*line;

	line = readline("heredoc> ");
	if (ft_strcmp(line, t->token) == 0)
	{
		free(line);
		return (1);
	}
	if (line && *line != '\n')
	{
		ft_write_heredoc(fd, line, new_environ, t);
		ft_putchar_fd('\n', fd);
		free (line);
	}
	else if (!line)
	{
		ft_message_ctrld(t->token);
		return (1);
	}
	return (0);
}

int	create_heredoc(t_token *t, char **new_environ)
{
	int		fd;
	pid_t	pid;
	int		r;

	if (t->type == HEREDOC_QUOTE)
		eliminate_quotes(&(t->token));
	pid = fork();
	r = 1;
	if (pid == -1)
		ft_error_fork();
	else if (pid == 0)
	{
		fd = ft_init_child();
		while (1)
			if (control_break(fd, t, new_environ))
				break ;
		close(fd);
		exit (1);
	}
	else
		waitpid(pid, &r, 0);
	return (r);
}
