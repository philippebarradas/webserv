/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodjian <dovdjianpro@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/20 14:31:52 by user42            #+#    #+#             */
/*   Updated: 2022/01/27 18:01:06 by dodjian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../treat_request.hpp"

std::string Treat_request::ft_post(std::string full_cmd,  const Parse_header & parse_head)
{
	std::cout << "je suis dans post !" << std::endl;
	(void)parse_head;
	return (full_cmd);
}
