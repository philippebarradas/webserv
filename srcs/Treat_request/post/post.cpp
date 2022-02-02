/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/20 14:31:52 by user42            #+#    #+#             */
/*   Updated: 2022/02/02 14:55:15 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../treat_request.hpp"

std::string Treat_request::ft_post(std::string full_cmd,  const Parse_request & parse_head)
{
	(void)parse_head;
	//std::cout << "[POST]" << std::endl;
	return (full_cmd + "[POST]");
}
