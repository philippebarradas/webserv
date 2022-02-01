/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodjian <dovdjianpro@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/20 14:31:52 by user42            #+#    #+#             */
/*   Updated: 2022/02/01 15:14:30 by dodjian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../treat_request.hpp"

std::string	getContent_Length(std::string body_request)
{
	std::string ret;
	int i = 0, j = 0;
	for (i = 0; body_request[i] && body_request[i] != '='; i++) {}
	std::cout << "i\t=\t" << i << std::endl;
	for (int j = 0; body_request[i + j]; j++){}
	std::cout << "j\t=\t" << j << std::endl;
	ret = int_to_string(j);
	return (ret);
}

std::string Treat_request::ft_post(std::string full_cmd,  const Parse_request & parse_head)
{
	std::cout << "je suis dans post !" << std::endl;
	std::cout << "body request = " << parse_head.get_request_body() << std::endl;
	this->_content_length = getContent_Length(parse_head.get_request_body());

	std::cout << "content length = " << this->_content_length << std::endl;
	return (full_cmd);
}
