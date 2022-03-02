/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 11:17:44 by dodjian           #+#    #+#             */
/*   Updated: 2022/03/02 11:31:21 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include "../Server/Engine.hpp"

class Server;
class Request;
class Engine;
class Parse_request;

class Cgi
{
	public:

		// CONSTRUCTOR
		Cgi(const std::string & root, const std::string & path,
			const std::string & pathCgi, const Parse_request & src_header,
				const Engine & src_engine);
		Cgi(const Cgi & src);

		// DESTRUCTOR
		~Cgi();

		// METHODS
		void	delete_argv_env(char **argv, char **env);
		std::string	normVar(std::string src);
		void	init_path(const std::string & root, const std::string & path,
			const std::string & pathCgi);
		void	init_env_client_var(const Parse_request & src_header);
		void	init_env_server_var();
		void	init_env_request_var(const Parse_request & src_header,
			const Engine & src_engine);
		void	init_env(const Parse_request & src_header,
			const Engine & src_engine);
		char	**convert_env();
		char	**create_argv(std::string const & path_file_executed);
		void	exec_cgi(char **argv, char **env,
			const Parse_request & src_header);
		std::string	body_response_from_fd(const int & fd);

		// GETTERS
		std::string	getSend_content() const;
		std::string	getType_Cgi() const;

		// OPERATORS
		Cgi &		operator=(const Cgi & rhs);
	private:

		// CONSTRUCTOR
		Cgi();

		// VARIABLES
		std::string							_type_cgi;
		std::string							_root;
		std::string							_path_file_executed_absolu;
		std::string							_path_file_executed;
		std::string							_path_cgi;
		std::string							_send_content;
		int									_pid;
		std::map<std::string, std::string>	_env;
};

#endif /* ************************************************************* CGI_H */
