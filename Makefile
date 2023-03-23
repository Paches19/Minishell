# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/10 12:11:32 by adpachec          #+#    #+#              #
#    Updated: 2023/03/23 16:03:14 by adpachec         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#Program name
NAME		=	minishell

# Paths
L_DIR		=	./libft/
O_DIR		=	./obj/
I_DIR		=	./include/

#Files
LIB_A		=	libft.a
SRCS_M		=	./srcs/minishell.c ./srcs/prompt.c

# Sources and objects
RM 			=	rm -rf
OBJS_M		=	$(patsubst $(S_DIR)%, $(O_DIR)%, $(SRCS_M:.c=.o))

HEADER		=	$(I_DIR)minishell.h
OBJS		=	$(OBJS_M)
S_DIR		=	./srcs/

INCL		= 
# -I$(HEADER) 
# -I ./vendor/readline/include

LIBFLAGS	=	-Llibft -lft -lreadline
#-L ./vendor/readline/lib

LEAKS		=	-fsanitize=address -g
W_FLAGS		=	-Wall -Wextra -Werror
LIB_N		=	$(L_DIR)$(LIB_A)

#  Colors
CYAN		=	\033[1;36m
YELLOW		=	\033[1;33m
GREEN		=	\033[0;32m
BLUE 		= 	\033[0;94m
RESET		=	\033[0m

# Rules
all			: $(NAME)

$(O_DIR)	:
				@echo "Creating program $(YELLOW)$(NAME)$(RESET)"
				@mkdir -p $(O_DIR)

$(O_DIR)%.o	:	$(S_DIR)%.c
				@echo "$(BLUE)Compiling $@ ! $(RESET)\c"
				@$(CC) $(W_FLAGS) $(INCL) -c $< -o $@
				@echo "... $(GREEN)OK$(RESET)" 

$(NAME) 	:	$(LIB_N) $(O_DIR) $(OBJS)           
				@echo "$(YELLOW)Linking object files ! $(RESET)\c"
				@$(CC) $(OBJS) $(LIBFLAGS) $(LEAKS) $(INCL) -o $(NAME)
				@echo "$(GREEN)SUCCESS !$(RESET)"
				@echo "$(NAME) created successfully !"

$(LIB_N)	:		
				@echo "Creating library $(YELLOW) $(LIB_A) $(RESET)"
				@$(MAKE) --no-print-directory -C $(L_DIR)

clean		:	
				@$(MAKE) --no-print-directory clean -C $(L_DIR) 
				@$(RM) $(O_DIR)
				@echo "$(CYAN)Deleted all the object files$(RESET)"

fclean		:	clean
				@$(MAKE) --no-print-directory fclean -C $(L_DIR)
				@$(RM) $(NAME)
				@echo "$(CYAN)Deleted all the exec files$(RESET)"

re			:	fclean all

.PHONY		:	all clean fclean re bonus