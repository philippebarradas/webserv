# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: user42 <user42@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/01/06 17:08:23 by tsannie           #+#    #+#              #
#    Updated: 2022/01/23 17:33:33 by user42           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

######################################################################
#                            Informations                            #
######################################################################

NAME			= webserv
CC				= @clang++
CFLAGS			=  #-std=c++98 #-Wall -Wextra -Werror
RM				= @rm -rf

######################################################################
#                               C0l0r$                               #
######################################################################

BLACK			:= $(shell tput -Txterm setaf 0)
RED				:= $(shell tput -Txterm setaf 1)
GREEN			:= $(shell tput -Txterm setaf 2)
YELLOW			:= $(shell tput -Txterm setaf 3)
LIGHTPURPLE		:= $(shell tput -Txterm setaf 4)
PURPLE			:= $(shell tput -Txterm setaf 5)
BLUE			:= $(shell tput -Txterm setaf 6)
WHITE			:= $(shell tput -Txterm setaf 7)
BOLD			:= $(shell tput -Txterm bold)
END				:= $(shell tput -Txterm sgr0)
NAMEC			= ${BLUE}${BOLD}$(NAME)${END}

######################################################################
#                            Source Files                            #
######################################################################

CONFIG			= Config.cpp Server.cpp utils.cpp

SERVER			= moteur.cpp

PARSE_HEADER	= parse_header.cpp checker_header.cpp

TREAT_REQUEST	= bad_request.cpp treat_request.cpp delete/delete.cpp \
					get/get.cpp post/post.cpp

AUTO_INDEX		= Autoindex.cpp

RESPONSE		= Response.cpp

MAIN			= main.cpp

SRC				= $(addprefix srcs/Config/, $(CONFIG)) \
				$(addprefix srcs/Server/, $(SERVER)) \
				$(addprefix srcs/Parse_header/, $(PARSE_HEADER)) \
				$(addprefix srcs/Treat_request/, $(TREAT_REQUEST)) \
				$(addprefix srcs/Autoindex/, $(AUTO_INDEX)) \
				$(addprefix srcs/Response/, $(RESPONSE)) \
				$(addprefix srcs/, $(MAIN))

######################################################################
#                            Object Files                            #
######################################################################

OBJ				= $(SRC:.cpp=.o)

######################################################################
#                               Rules                                #
######################################################################

%.o: %.cpp
			@printf "${PURPLE}${BOLD}Compile: %-35.35s\r${END}" $@
			${CC} ${CFLAGS} -c $< -o $@

all:
			@printf "${PURPLE}${BOLD}Start compile ...\n${END}"
			@$(MAKE) $(NAME)

$(NAME): 	$(OBJ)
			$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
			@echo "\n\n${GREEN}The $(NAMEC) ${GREEN}has been build !${END}"

clean:
			$(RM) $(OBJ)
			@echo "${LIGHTPURPLE}Cleaning ...${END}\n"

fclean:		clean
			$(RM) $(NAME)
			@echo "${LIGHTPURPLE}Delete $(NAMEC)${LIGHTPURPLE}...${END}\n"

re:			fclean all

.PHONY:		all test clean fclean re
