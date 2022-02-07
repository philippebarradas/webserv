/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 11:17:44 by dodjian           #+#    #+#             */
/*   Updated: 2022/02/07 14:57:18 by tsannie          ###   ########.fr       */
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
#include <ext/stdio_filebuf.h>

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
class Treat_request;

class Cgi
{
	public:

		// CONSTRUCTOR
		Cgi();
		Cgi(const Server & src, const Parse_request & src_header, const Engine & src_engine, const Treat_request & src_request); // pour l'instant
		//Cgi(const Request & re); pour plus tard
		Cgi(Cgi const & src);

		// DESTRUCTOR
		~Cgi();

		// METHODS
		void	delete_argv_env(char **argv, char **env);
		std::string	to_string(size_t nb);
		bool	is_file_cgi(std::string path_extension);
		void	init_path(const Server & src);
		void	init_env_client_var(const Server & src, const Parse_request & src_header,
			const Treat_request & src_request);
		void	init_env_server_var(const Server & src, const Parse_request & src_header);
		void	init_env_request_var(const Server & src, const Parse_request & src_header,
			const Engine & src_engine, const Treat_request & src_request);
		void	init_env(const Server & src, const Parse_request & src_header,
			const Engine & src_engine, const Treat_request & src_request);
		char	**convert_env(std::map<std::string, std::string>);
		char	**create_argv(std::string path_file_executed);
		void	exec_cgi(char **argv, char **env, const Parse_request & src_header, const Treat_request & src_request);
		std::string	fd_to_string(int fd);
		void	write_body_post_in_fd(std::string body_string); // body | php-cgi

		// GETTERS
		std::map<std::string, std::string>	getEnv() const;
		std::string	getSend_content() const;
		std::string	getPath_cgi() const;
		std::string	getUser() const;
		std::string	getHome() const;
		int	getPid() const;

		// OPERATORS
		Cgi &		operator=( Cgi const & rhs );

	private:

		// VARIABLES
		std::string	_path_cgi;
		std::string	_user;
		std::string	_home;
		std::string	_send_content;
		int	_pid;
		std::map<std::string, std::string>	_env;
};

std::ostream &			operator<<( std::ostream & o, Cgi const & i );

#endif /* ************************************************************* CGI_H */
