/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 09:32:53 by tsannie           #+#    #+#             */
/*   Updated: 2022/01/19 10:27:44 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Autoindex.hpp"

int	main(int ac, char **av)
{
	std::string	path("/home/user42/Bureau/webserv/srcs/Config/default/html_page");
	std::cout << "path\t=\t" << path << std::endl;
	std::cout << std::endl;

	Autoindex	test(path.c_str());

	std::cout << test << std::endl;
}
