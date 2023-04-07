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
NAME := minishell

# Project directories
SRCDIR := srcs
INCDIR := include
OBJDIR := obj
LIBDIR := libft

# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -Werror

# Para los MAC de 42
# INCLUDES := -I$(INCDIR) -I/Users/jutrera-/.brew/opt/readline/include
# LDFLAGS := -L ./libft/ -lft -L/Users/jutrera-/.brew/opt/readline/lib/ -lreadline

# Para mi Windows
INCLUDES := -I$(INCDIR)
LDFLAGS := -L ./libft/ -lft -lreadline

LEAKS := -fsanitize=address -g

# Source files
SRCS := $(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/builtins/*.c) \
	$(wildcard $(SRCDIR)/environ/*.c) $(wildcard $(SRCDIR)/prompt/*.c) \
	$(wildcard $(SRCDIR)/style/*.c) $(wildcard $(SRCDIR)/tokenization/*.c) \
	$(wildcard $(SRCDIR)/utils/*.c) $(wildcard $(SRCDIR)/commands/*.c) \
	$(wildcard *.c)
# Object files
OBJS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

# Libraries
LIBRARY := $(LIBDIR)/libft.a

# Color codes for terminal output
RED := \033[0;31m
GREEN := \033[0;32m
YELLOW := \033[0;33m
RESET := \033[0m

# Delete files
RM := rm -rf

# Default target
all: $(NAME)

# Compile object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) $(LEAKS) $(INCLUDES) -c $< -o $@
	@echo "$(GREEN)Compiled $@ successfully!$(RESET)"

# Compile library
$(LIBRARY):
	@echo "$(YELLOW)Compiling library...$(RESET)"
	@$(MAKE) --no-print-directory -C $(LIBDIR)

# Link program
$(NAME): $(LIBRARY) $(OBJS)
	@echo "$(YELLOW)Linking $(NAME)...$(RESET)"
	@$(CC) $(OBJS) $(CFLAGS) $(LEAKS) $(LDFLAGS) -o $(NAME)
	@echo "$(GREEN)$(NAME) created successfully!$(RESET)"

# Clean object files and program
clean:
	@echo "$(RED)Deleting object files...$(RESET)"
	@$(RM) $(OBJDIR)
	@echo "$(RED)Deleting $(NAME)...$(RESET)"
	@$(RM) $(NAME)

# Clean everything and recompile
fclean: clean
	@echo "$(RED)Deleting library...$(RESET)"
	@$(MAKE) fclean --no-print-directory -C $(LIBDIR)
	@echo "$(GREEN)Finished cleaning!$(RESET)"

# Recompile everything
re: fclean all

# Prevent errors if object files are deleted
-include $(OBJS:.o=.d)

# Phony targets
.PHONY: all clean fclean re
