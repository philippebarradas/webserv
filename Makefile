# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
<<<<<<< HEAD
#    By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/01/06 17:08:23 by tsannie           #+#    #+#              #
#    Updated: 2022/02/07 14:56:55 by tsannie          ###   ########.fr        #
=======
#    By: user42 <user42@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/01/06 17:08:23 by tsannie           #+#    #+#              #
#    Updated: 2022/02/07 12:25:20 by user42           ###   ########.fr        #
>>>>>>> parse_header_path
#                                                                              #
# **************************************************************************** #

######################################################################
#                            Informations                            #
######################################################################

NAME			= webserv
CC				= @clang++
CFLAGS			= #-Wall -Wextra -Werror -std=c++98
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

CGI			= Cgi.cpp

CONFIG		= Config.cpp Server.cpp utils.cpp

SERVER			= Engine.cpp

PARSE_REQUEST	= parse_request.cpp checker_request.cpp body.cpp

AUTO_INDEX		= Autoindex.cpp

RESPONSE		= Response.cpp TreatRequest.cpp

MAIN			= main.cpp

SRC				= $(addprefix srcs/Config/, $(CONFIG)) \
				$(addprefix srcs/Cgi/, $(CGI)) \
				$(addprefix srcs/Server/, $(SERVER)) \
				$(addprefix srcs/Parse_request/, $(PARSE_REQUEST)) \
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
