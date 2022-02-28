/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodjian <dovdjianpro@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 11:17:37 by dodjian           #+#    #+#             */
/*   Updated: 2022/02/28 11:44:45 by dodjian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Cgi::Cgi()
{
}

Cgi::Cgi(std::string const & root, std::string const & path,
	std::string const & pathCgi, Parse_request const & src_header,
		Engine const & src_engine)
{
	init_path(root, path, pathCgi);
	init_env(src_header, src_engine);
}

Cgi::Cgi( const Cgi & src )
{
	*this = src;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Cgi::~Cgi()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Cgi &				Cgi::operator=( Cgi const & rhs )
{
	if ( this != &rhs )
	{
		this->_path_cgi = rhs.getPath_cgi();
		this->_send_content = rhs.getSend_content();
		this->_env = rhs.getEnv();
		this->_pid = rhs.getPid();
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void	Cgi::delete_argv_env(char **argv, char **env)
{
	for (int i = 0; env[i]; i++)
		delete [] env[i];
	for (int i = 0; argv[i]; i++)
		delete [] argv[i];
	delete [] env;
	delete [] argv;
}

void	Cgi::init_path(std::string const & root, std::string const & path,
	std::string const & pathCgi)
{
	this->_path_file_executed = path;
	this->_path_cgi = pathCgi;
	this->_root = root;
	this->_path_file_executed_absolu =
		this->_path_file_executed.substr(this->_root.size(),
			this->_path_file_executed.size());
	this->_user = "user42";
}

std::string	Cgi::normVar( std::string src )
{
	for (size_t i = 0 ; src[i] ; ++i)
	{
		src[i] = std::toupper(src[i]);
		if (src[i] == '-')
			src[i] = '_';
	}
	return ("HTTP_" + src);
}

void	Cgi::init_env_client_var(const Parse_request & src_header)
{
	std::map<std::string, std::string>::const_iterator	it, end;

	end = src_header.get_param_request_tab().end();
	for (it = src_header.get_param_request_tab().begin(); it != end ; ++it)
	{
		this->_env[this->normVar(it->first)] = it->second;
	}
}

void	Cgi::init_env_server_var(const Parse_request & src_header)
{
	this->_env["USER"] = this->_user;
	this->_env["SERVER_SOFTWARE"] = "webserv/1.0";
	this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
}

void	Cgi::init_env_request_var(const Parse_request & src_header,
	const Engine & src_engine)
{
	if (src_header.get_request("Query") != "")
		this->_env["REQUEST_URI"] = this->_path_file_executed_absolu + '?' +
			src_header.get_request("Query");
	else
		this->_env["REQUEST_URI"] = this->_path_file_executed_absolu;
	this->_env["SCRIPT_FILENAME"] = this->_path_file_executed;
	this->_env["DOCUMENT_ROOT"] = this->_root;
	this->_env["DOCUMENT_URI"] = this->_path_file_executed_absolu;
	this->_env["SERVER_PROTOCOL"] = src_header.get_request("Protocol");
	this->_env["SERVER_PORT"] = sizet_to_string(src_engine.GetAccessPort());
	this->_env["REQUEST_METHOD"] = src_header.get_request("Method");
	this->_env["SCRIPT_NAME"] = this->_path_file_executed_absolu;
	this->_env["QUERY_STRING"] = src_header.get_request("Query");
	this->_env["REMOTE_PORT"] = src_engine.GetRemote_Port();
	this->_env["REMOTE_ADDR"] = src_engine.GetRemote_Addr();
	this->_env["CONTENT_TYPE"] = src_header.get_request("Content-Type:");
	this->_env["CONTENT_LENGTH"] =
		sizet_to_string(src_header.get_request_body_size());
	this->_env["REDIRECT_STATUS"] = src_header.get_request("Status");
}

void	Cgi::init_env(Parse_request const & src_header,
	Engine const & src_engine)
{
	std::map<std::string, std::string>::iterator it_env;

	init_env_client_var(src_header);
	init_env_server_var(src_header);
	init_env_request_var(src_header, src_engine);
}

char **Cgi::convert_env(std::map<std::string, std::string>)
{
	std::map<std::string, std::string>::const_iterator it_env;
	char	**env = new char *[this->_env.size() + 1];
	int	j = 0;
	for (it_env = this->_env.begin(); it_env != this->_env.end(); it_env++)
	{
		std::string	content = it_env->first + "=" + it_env->second;
		env[j] = new char[content.size() + 1];
		env[j] = strcpy(env[j], content.c_str());
		j++;
	}
	env[j] = NULL;
	return (env);
}

char	**Cgi::create_argv(std::string path_file_executed)
{
	int		nbr_argv = 2;
	char	**argv = new char *[nbr_argv + 1];

	argv[0] = new char[1];
	strcpy(argv[0], "");
	argv[1] = new char[path_file_executed.size() + 1];
	strcpy(argv[1], path_file_executed.c_str());
	argv[2] = NULL;
	return (argv);
}

void	Cgi::exec_cgi(char **argv, char **env,
	Parse_request const & src_header)
{
	std::string	body_string = src_header.get_request_body();
	int			real_stdin = dup(STDIN_FILENO);
	int			real_stdout = dup(STDOUT_FILENO);
	FILE		*file_stdin = tmpfile();
	FILE		*file_stdout = tmpfile();
	int			fd_stdin = fileno(file_stdin);
	int			fd_stdout = fileno(file_stdout);
	int			status = 0;

	this->_pid = fork();
	if (this->_pid == -1)
		throw std::runtime_error("[Error] pid failed");
	else if (this->_pid == 0)
	{
		if (src_header.get_request("Method") == "POST")
		{
			std::fputs(body_string.c_str(), file_stdin);
			rewind(file_stdin);
		}
		dup2(fd_stdin, STDIN_FILENO);
		dup2(fd_stdout, STDOUT_FILENO);
		if (execve(this->_path_cgi.c_str(), argv, env) == -1)
			throw std::runtime_error("[Error] execve failed");
	}
	waitpid(this->_pid, &status, 0);
	lseek(fd_stdout, 0, SEEK_SET);
	this->_send_content = body_response_from_fd(fd_stdout);
	dup2(real_stdin, STDIN_FILENO);
	dup2(real_stdout, STDOUT_FILENO);
	fclose(file_stdin);
	fclose(file_stdout);
	close(fd_stdin);
	close(fd_stdout);
	close(real_stdin);
	close(real_stdout);
	delete_argv_env(argv, env);
}

std::string	Cgi::body_response_from_fd(int fd)
{
	__gnu_cxx::stdio_filebuf<char> filebuf(fd, std::ios::in);
	std::istream is(&filebuf);
	std::string ret, line;
	while (std::getline(is, line))
	{
		ret += line;
		ret += '\n';
	}
	ret += '\n';
	return (ret);
}



/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::map<std::string, std::string> Cgi::getEnv() const
{
	return (this->_env);
}

std::string	Cgi::getSend_content() const
{
	return (this->_send_content);
}

std::string	Cgi::getPath_cgi() const
{
	return (this->_path_cgi);
}

std::string	Cgi::getUser() const
{
	return (this->_user);
}

std::string	Cgi::getHome() const
{
	return (this->_home);
}

int	Cgi::getPid() const
{
	return (this->_pid);
}

std::string	Cgi::getType_Cgi() const
{
	return (this->_type_cgi);
}
/* ************************************************************************** */
