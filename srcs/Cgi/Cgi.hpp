/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodjian <dovdjianpro@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 11:17:44 by dodjian           #+#    #+#             */
/*   Updated: 2022/02/16 17:35:43 by dodjian          ###   ########.fr       */
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
#include "../Server/Engine.hpp"
#include "../Config/Server.hpp"

class Server;
class Request;
class Engine;
class Parse_request;

class Cgi
{
	public:

		// CONSTRUCTOR
		Cgi();
		Cgi(std::string const & root, std::string const & path, std::string const & pathCgi, const Parse_request & src_header, const Engine & src_engine); // pour l'instant
		Cgi(Cgi const & src);

		// DESTRUCTOR
		~Cgi();

		// METHODS
		void	delete_argv_env(char **argv, char **env);
		std::string	to_string(size_t nb);
		bool	is_file_cgi(std::string path_extension);
		void	init_path(std::string const & root, std::string const & path, std::string const & pathCgi);
		void	init_env_client_var(const Parse_request & src_header);
		void	init_env_server_var(const Parse_request & src_header);
		void	init_env_request_var(const Parse_request & src_header,
			const Engine & src_engine);
		void	init_env(const Parse_request & src_header,
			const Engine & src_engine);
		char	**convert_env(std::map<std::string, std::string>);
		char	**create_argv(std::string path_file_executed);
		void	exec_cgi(char **argv, char **env, const Parse_request & src_header);
		std::string	body_response_from_fd(int fd);
		void	write_body_post_in_fd(std::string body_string); // body | php-cgi
		void	upload_file(std::string response);

		// GETTERS
		std::map<std::string, std::string>	getEnv() const;
		std::string	getSend_content() const;
		std::string	getPath_cgi() const;
		std::string	getUser() const;
		std::string	getHome() const;
		std::string	getType_Cgi() const;
		int	getPid() const;

		// OPERATORS
		Cgi &		operator=( Cgi const & rhs );

	private:

		// VARIABLES
		std::string	_type_cgi;
		std::string	_root; // 1
		std::string	_path_file_executed_absolu; // 2
		std::string	_path_file_executed; // 1 + 2
		std::string	_path_cgi;
		std::string	_user;
		std::string	_home;
		std::string	_send_content;
		int	_pid;
		std::map<std::string, std::string>	_env;
};

std::ostream &			operator<<( std::ostream & o, Cgi const & i );

#endif /* ************************************************************* CGI_H */
