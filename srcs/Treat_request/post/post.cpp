/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodjian <dovdjianpro@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/20 14:31:52 by user42            #+#    #+#             */
/*   Updated: 2022/02/01 18:29:40 by dodjian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../treat_request.hpp"

std::string	Treat_request::get_body_request() const
{
	return (this->_body_request);
}

std::string Treat_request::ft_post(std::string full_cmd, const Parse_request & parse_head)
{
	std::cout << "je suis dans post !" << std::endl;
	return (full_cmd);
}
