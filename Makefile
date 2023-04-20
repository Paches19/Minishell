# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/24 13:43:11 by adpachec          #+#    #+#              #
#    Updated: 2023/03/30 12:27:12 by adpachec         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Program name
NAME 		=	minishell

# Project directories
SRCDIR 		=	srcs
INCDIR 		=	include
OBJDIR 		=	obj
LIBDIR 		=	libft

# Compiler and flags
CC 			=	gcc
CFLAGS 		=	-Wall -Wextra -Werror

# Para los MAC de 42
# INCLUDES 	= -I$(INCDIR) -I/Users/jutrera-/.brew/opt/readline/include
# LDFLAGS 	= -L ./libft/ -lft -L/Users/jutrera-/.brew/opt/readline/lib/ -lreadline

# Para los MAC de 42
INCLUDES 	= -I$(INCDIR) -I ./vendor/readline/include
LDFLAGS 	= -L ./libft/ -lft -L ./vendor/readline/lib -lreadline

# # Para mi Windows
# INCLUDES	=	-I$(INCDIR)
# LDFLAGS 	=	-L ./libft/ -lft -lreadline

LEAKS 		=	-fsanitize=address -g

# Source files
SRCS		=	srcs/main.c \
				srcs/builtins/exec_builtins.c \
				srcs/builtins/ft_cd.c \
				srcs/builtins/ft_echo.c \
				srcs/builtins/ft_echo_utils.c \
				srcs/builtins/ft_env.c \
				srcs/builtins/ft_exit.c \
				srcs/builtins/ft_export_utils.c \
				srcs/builtins/ft_export.c \
				srcs/builtins/ft_pwd.c \
				srcs/builtins/ft_unset.c \
				srcs/commands/create_heredoc.c \
				srcs/commands/create_heredoc_utils.c \
				srcs/commands/exec_one_command.c \
				srcs/commands/execute_commands.c \
				srcs/commands/get_cmd.c \
				srcs/commands/get_paths_acces.c \
				srcs/commands/init_pipe_struct.c \
				srcs/commands/init_pipe_struct_utils.c \
				srcs/commands/pipex.c \
				srcs/environ/copy_environ.c \
				srcs/environ/count_vars.c \
				srcs/environ/env_in_order.c \
				srcs/environ/ft_check_vars.c \
				srcs/environ/ft_update_double_quote.c \
				srcs/environ/get_env.c \
				srcs/prompt/renewprompt.c \
				srcs/style/splash.c \
				srcs/tokenization/check_builtin.c \
				srcs/tokenization/ft_reading_token.c \
				srcs/tokenization/read_utils.c \
				srcs/tokenization/sort_tokens.c \
				srcs/tokenization/token_utils.c \
				srcs/tokenization/tokenize_input.c \
				srcs/utils/free.c \
				srcs/utils/ft_errors.c \
				srcs/utils/ft_init_matrix.c \
				srcs/utils/ft_is.c

# Object files
OBJS 		= 	$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

# Libraries
LIBRARY 	= 	$(LIBDIR)/libft.a

# Color codes for terminal output
RED 		= 	\033[0;31m
GREEN	 	= 	\033[0;32m
YELLOW 		= 	\033[0;33m
RESET 		= 	\033[0m

# Delete files
RM 			= 	rm -rf

# Default target
all			: 	$(NAME)

# Compile object files
$(OBJDIR)/%.o: 	$(SRCDIR)/%.c
				@mkdir -p $(dir $@)
				@echo "Compiling $(YELLOW)$<$(RESET)...\c"
				@$(CC) $(CFLAGS) $(LEAKS) $(INCLUDES) -c $< -o $@
				@echo "$(GREEN)OK !$(RESET)"

# Compile library
$(LIBRARY)	:
				@echo "$(YELLOW)Compiling library...$(RESET)"
				@$(MAKE) --no-print-directory -C $(LIBDIR)
				@echo "\n$(YELLOW)Compiling programme !$(RESET)"

# Link program
$(NAME)		: 	$(LIBRARY) $(OBJS)
				@echo "Linking object files... \c"
				@$(CC) $(OBJS) $(CFLAGS) $(LEAKS) $(LDFLAGS) -o $(NAME)
				@echo "$(GREEN)$(NAME) created successfully !$(RESET)"

# Clean object files
clean		:
				@echo "$(RED)Deleting object files...$(RESET)\c"
				@$(MAKE) clean --no-print-directory -C $(LIBDIR) 
				@$(RM) $(OBJDIR)
				@echo "$(GREEN) all object files DELETED !$(RESET)"

# Clean everything and recompile
fclean		: 	clean
				@echo "$(RED)Deleting object and binary files...$(RESET)\c"
				@$(MAKE) fclean --no-print-directory -C $(LIBDIR)
				@$(RM) $(NAME)
				@echo "$(GREEN)all files DELETED !$(RESET)"

# Recompile everything
re			: 	fclean all

# Prevent errors if object files are deleted
-include $(OBJS:.o=.d)

# Phony targets
.PHONY		: 	all clean fclean re
