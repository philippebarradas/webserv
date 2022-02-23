/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodjian <dovdjianpro@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 11:17:37 by dodjian           #+#    #+#             */
/*   Updated: 2022/01/18 15:34:51 dodjian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Cgi::Cgi()
{
}

Cgi::Cgi(std::string const & root, std::string const & path, std::string const & pathCgi, const Parse_request & src_header, const Engine & src_engine)
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

std::ostream &			operator<<( std::ostream & o, Cgi const & i )
{
	(void)i;
	//o << "Value = " << i.getValue();
	return o;
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

void	Cgi::init_path(std::string const & root, std::string const & path, std::string const & pathCgi)
{
	this->_path_file_executed = path;
	this->_path_cgi = pathCgi;
	this->_root = root;
	this->_path_file_executed_absolu =
		this->_path_file_executed.substr(this->_root.size(), this->_path_file_executed.size());
	this->_user = "user42";
	//this->_home = "/home/user42/Bureau/webserv/www";
}

std::string	Cgi::normVar( std::string src )
{
	for (size_t i = 0 ; src[i] ; ++i)
	{
		toupper(src[i]);
		if (src[i] == '-')
			src[i] = '_';
	}
	return (src);
}

// var from client
void	Cgi::init_env_client_var(const Parse_request & src_header)
{
	std::map<std::string, std::string>::const_iterator	it, end;

	// end = .end();
	for (it ; it != end ; ++it)
	{
		this->_env[this->normVar(it->first)] = it->second;
	}

	/*this->_env["HTTP_ACCEPT"] = src_header.get_request("Accept:");
	this->_env["HTTP_ACCEPT_LANGUAGE"] = src_header.get_request("Accept-Language:");
	this->_env["HTTP_ACCEPT_ENCODING"] = src_header.get_request("Accept-Encoding:");
	this->_env["HTTP_SEC_FETCH_DEST"] = src_header.get_request("Sec-Fetch-Dest:");
	this->_env["HTTP_SEC_FETCH_USER"] = src_header.get_request("Sec-Fetch-User:");
	this->_env["HTTP_USER_AGENT"] = src_header.get_request("User-Agent:");
	this->_env["HTTP_CONNECTION"] = src_header.get_request("Connection:");
	if (src_header.get_request("Method").compare("POST") == 0)
	{
		this->_env["HTTP_CONTENT_LENGTH"] = src_header.get_request("Content-Length:");
		this->_env["HTTP_CONTENT_TYPE"] = src_header.get_request("Content-Type:");
	}
	this->_env["HTTP_REFERER"] = src_header.get_request("Referer:");*/
}

// var server
void	Cgi::init_env_server_var(const Parse_request & src_header)
{
	this->_env["HOME"] = this->_home; // pas sur
	this->_env["USER"] = this->_user; // pas sur
	this->_env["SERVER_SOFTWARE"] = "webserv/1.0";
	//this->_env["SERVER_NAME"] = src_header.get_request("Host:");
	this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
}

// var request
void	Cgi::init_env_request_var(const Parse_request & src_header, const Engine & src_engine)
{
	/* difference entre GET et POST sur un form html qui redirect ur env.php:
		POST par rapport a get: + : HTTP_CONTENT_LENGTH, HTTP_CONTENT_TYPE == CONTENT_TYPE et CONTENT_LENGTH
		- : QUERY_STRING vide et du coup REQUEST_URI vide
	*/
	// remplacer "/env.php" par le bon fichier apres traitement de requete:
	//this->_env["AUTH_TYPE"] = "HTTP";
	//this->_env["REQUEST_SCHEME"] = "http";
	if (src_header.get_request("Cookie:") != "")
		this->_env["HTTP_COOKIE"] = src_header.get_request("Cookie:");	// debrouille toi avec ca. La bise !
	if (src_header.get_request("Query").compare(""))
		this->_env["REQUEST_URI"] = this->_path_file_executed_absolu + '?' + src_header.get_request("Query");//query string ;
	else
		this->_env["REQUEST_URI"] = this->_path_file_executed_absolu;
	this->_env["SCRIPT_FILENAME"] = this->_path_file_executed;
	this->_env["DOCUMENT_ROOT"] = this->_root;
	this->_env["DOCUMENT_URI"] = this->_path_file_executed_absolu;
	this->_env["SERVER_PROTOCOL"] = src_header.get_request("Protocol");
	this->_env["SERVER_PORT"] = sizet_to_string(src_engine.GetAccessPort());
	this->_env["REQUEST_METHOD"] = src_header.get_request("Method"); // pas bien
	this->_env["SCRIPT_NAME"] = this->_path_file_executed_absolu;
	this->_env["QUERY_STRING"] = src_header.get_request("Query");
	std::cout << YELLOW << "src_header.get_request(Query);=[" << src_header.get_request("Query") << "]" << END << std::endl;
	this->_env["REMOTE_PORT"] = src_engine.GetRemote_Port();
	this->_env["REMOTE_ADDR"] = src_engine.GetRemote_Addr();
	this->_env["CONTENT_TYPE"] = src_header.get_request("Content-Type:");
	this->_env["CONTENT_LENGTH"] = sizet_to_string(src_header.get_request_body_size());
	//this->_env["CONTENT_LENGTH"] = src_header.get_request_body_size();
	this->_env["REDIRECT_STATUS"] = src_header.get_request("Status");
}

void	Cgi::init_env(const Parse_request & src_header, const Engine & src_engine)
{
	std::map<std::string, std::string>::iterator it_env;

	init_env_client_var(src_header);
	init_env_server_var(src_header);
	init_env_request_var(src_header, src_engine);
	//for (it_env = this->_env.begin(); it_env != this->_env.end(); it_env++)
		//std::cout << PURPLE << it_env->first << " = " << BLUE << it_env->second << std::endl << END;
}

char **Cgi::convert_env(std::map<std::string, std::string>)
{
	std::map<std::string, std::string>::const_iterator it_env;
	char	**env = new char *[this->_env.size() + 1];
	int	j = 0;
	for (it_env = this->_env.begin(); it_env != this->_env.end(); it_env++)
	{
		std::string	content = it_env->first + "=" + it_env->second;
		//std::cout << "coontent =" << content << std::endl;
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

void	Cgi::exec_cgi(char **argv, char **env, const Parse_request & src_header)
{
	std::string body_string = src_header.get_request_body();
	int		real_stdin = dup(STDIN_FILENO);
	int		real_stdout = dup(STDOUT_FILENO);
	FILE	*file_stdin = tmpfile();
	FILE	*file_stdout = tmpfile();
	int	fd_stdin = fileno(file_stdin);
	int	fd_stdout = fileno(file_stdout);
	int		status = 0;
	this->_pid = fork();

	if (this->_pid == -1)
		std::cout << RED << "FAIL PID -1" << END << std::endl;
	else if (this->_pid == 0)
	{		
		if (src_header.get_request("Method").compare("POST") == 0)
		{
			write(fd_stdin, body_string.c_str(),body_string.size());
			lseek(fd_stdin, 0, SEEK_SET);
		}
		dup2(fd_stdin, STDIN_FILENO);
		dup2(fd_stdout, STDOUT_FILENO);
		if (execve(this->_path_cgi.c_str(), argv, env) == -1)
			std::cout << "error execve cgi" << std::endl;
	}	
	//std::cout << "{waitpid}" << std::endl;
	//std::cout << RED << "this->_pid=[" << this->_pid << "]" << END << std::endl;
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
	std::string content_type = "Content-type:";
	while (std::getline(is, line))
	{
		if (line.compare(0, content_type.size(), content_type) == 0)
			this->_type_cgi = line;
		else
		{
			ret += line;
			ret += '\n';
			//std::cout << CYAN << "line=[" << line << "]" << END << std::endl;
		}
	}
	//std::cout << "this->_type_cgi\t=\t" << this->_type_cgi << std::endl;
	return (ret);
}

void	Cgi::upload_file(std::string response)
{
	//response = "0123456789\n";
	//std::cout << "Je suis dans upload" << std::endl;
	std::ofstream out("www/uploads/file_created.txt");

	//std::cout << GREEN << "response = " << std::endl << "|" <<
	//response << "|" << std::endl << END;
	out << response;
	out.close();
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
