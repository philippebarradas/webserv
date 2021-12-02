/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   option.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phbarrad <phbarrad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 12:56:02 by phbarrad          #+#    #+#             */
/*   Updated: 2021/12/02 12:56:37 by phbarrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int ft_option(int ac, char **av)
{
	if (av[1][0] == '-')
	{
		if (strcmp(av[1], "-help") == 0)
		{
			std::cout << "webserv version: webserv/1.0" << std::endl << 
			"Usage: webserv [-?hvVtTq] [-s signal] [-p prefix] " << std::endl <<
			"[-e filename] [-c filename] [-g directives] " << std::endl <<
			"Options:" << std::endl <<
			"-?,-h         : this help" << std::endl << "-v            : show version and exit" << std::endl <<
			"-V            : show version and configure options then exit" << std::endl <<
			"-t            : test configuration and exit" << std::endl <<
			"-T            : test configuration, dump it and exit" << std::endl <<
			"-q            : suppress non-error messages during configuration testing" << std::endl <<
			"-s signal     : send signal to a master process: stop, quit, reopen, reload" << std::endl << 
			"-p prefix     : set prefix path (default: /Users/phbarrad/.brew/Cellar/webserv/1.21.4/)" << std::endl << 
			"-e filename   : set error log file (default: /Users/phbarrad/.brew/var/log/webserv/error.log)" << std::endl << 
			"-c filename   : set configuration file (default: /Users/phbarrad/.brew/etc/webserv/webserv.conf)" << std::endl << 
			"-g directives : set global directives out of configuration file" << std::endl;
			return (1);
		}
	}
	return (0);
}