/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodjian <dovdjianpro@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 11:17:44 by dodjian           #+#    #+#             */
/*   Updated: 2022/01/18 17:15:47 by dodjian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

// C++
#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <vector>
#include <map>

// C
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

// Colors
#define PURPLE	"\033[0;35m"
#define BLACK	"\033[1;30m"
#define RED		"\033[1;31m"
#define GREEN	"\033[1;32m"
#define YELLOW	"\033[1;33m"
#define BLUE	"\033[1;34m"
#define PURPLE2 "\033[1;35m"
#define CYAN	"\033[1;36m"
#define WHITE	"\033[1;37m"
#define END		"\033[0m"

// Utils macro
#define TRUE 1
#define FALSE 0

// My class
#include "../Config/Server.hpp"

class Server;
class Request;

class Cgi
{
	public:
		Cgi();
		Cgi(const Server & src, const Parse_header & src_header); // pour l'instant
		//Cgi(const Request & re); pour plus tard
		Cgi(Cgi const & src);
		~Cgi();

		std::string	to_string(size_t nb);
		bool	is_cgi(const Parse_header & src_header);
		void	init_env(const Server & src, const Parse_header & src_header);
		char	**convert_env(std::map<std::string, std::string>);
		char	**create_argv();
		void	exec_cgi(const Server & src, char **argv, char **env);
		std::string	redirect_result_cgi(int pipefd[2]);
		std::map<std::string, std::string>	getEnv() const;
		std::string	getSend_content() const;
		Cgi &		operator=( Cgi const & rhs );

	private:
		std::string	_send_content;
		int	_pid;
		std::map<std::string, std::string>	_env;
};

std::ostream &			operator<<( std::ostream & o, Cgi const & i );

#endif /* ************************************************************* CGI_H */
