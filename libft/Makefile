# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jutrera- <jutrera-@student.42madrid.com>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/09/27 18:05:52 by jutrera-          #+#    #+#              #
#    Updated: 2022/10/15 11:35:42 by jutrera-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	libft.a
CC			=	gcc
CFLAGS		=	-Wall -Wextra -Werror 

I_DIR		=	include/
O_DIR		=	obj/
S_DIR		=	srcs/
SRCS		=	$(wildcard $(S_DIR)*.c)
OBJS		=	$(patsubst $(S_DIR)%, $(O_DIR)%, $(SRCS:.c=.o))

NRM 		= 	\033[0;39m
RED 		= 	\033[0;91m
GREEN 		= 	\033[0;92m
YELLOW 		= 	\033[0;93m
BLUE 		= 	\033[0;94m

all			:	${NAME}

$(O_DIR)	:
				@mkdir -p $(O_DIR)

$(O_DIR)%.o	:	$(S_DIR)%.c
				@echo "$(BLUE)Compiling $@ !... $(NRM)\c"
				@${CC} ${CFLAGS} -I$(I_DIR) -c $< -o $@
				@echo "$(GREEN)OK$(NRM)"

${NAME}		:	$(O_DIR) $(OBJS)
				@echo "$(YELLOW)Linking object files ! $(NRM)\c"
				@ar rcs ${NAME} ${OBJS}
				@echo "$(GREEN)SUCCESS !$(NRM)"
				@ranlib $(NAME)
				@echo "libft.a created successfully !"

clean		:
				@rm -Rf $(O_DIR)/

fclean		:	clean
				@rm -f ${NAME}

re			:	fclean all

.PHONY		:	all clean fclean re
