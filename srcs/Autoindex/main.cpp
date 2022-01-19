/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 09:32:53 by tsannie           #+#    #+#             */
/*   Updated: 2022/01/19 11:07:20 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Autoindex.hpp"

int	main(int ac, char **av)
{
	std::string	root("/home/user42/Bureau/webserv/srcs/Config/default/html_page");
	std::string	path("/");

	std::cout << "path\t=\t" << path << std::endl;
	std::cout << "root\t=\t" << root << std::endl;
	std::cout << std::endl;

	Autoindex	test(root.c_str(), path);

	std::cout << test << std::endl;
}
