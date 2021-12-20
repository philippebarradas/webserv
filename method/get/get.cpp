/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/20 14:31:46 by user42            #+#    #+#             */
/*   Updated: 2021/12/20 16:43:33 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../method.hpp"
#include <vector>

std::string Method::ft_get(std::string full_cmd)//, [loca])
{/* 
    std::cout << "Full_cmd = [" << full_cmd << "]" << std::endl;   
    size_t pos;

    std::string request_method = full_cmd.substr(0, full_cmd.find(" "));
    pos = full_cmd.find(" ");
    full_cmd = full_cmd.substr(pos + 1);

    std::string path_info = full_cmd.substr(0, full_cmd.find(" "));
    pos = full_cmd.find(" ");
    full_cmd = full_cmd.substr(pos + 1);
   
    std::string server_protocol = full_cmd.substr(0, full_cmd.find(" "));
    pos = full_cmd.find(" ");
    full_cmd = full_cmd.substr(pos + 1);




    std::cout << "request_method = [" << request_method << "]" << std::endl;
    std::cout << "path_info = [" << path_info << "]" << std::endl;
    std::cout << "server_protocol = [" << server_protocol << "]" << std::endl; */

    //path.


    //std::string http = NULL;    

    //std::cout << "path = [" << path << "]" << std::endl;   
   // std::cout << "http = [" << http << "]" << std::endl;   
   // std::cout << "Full_cmd = [" << full_cmd << "]" << std::endl;   
	return ("HTTP/1.1 200 Good Request\nServer: localhost:12345/\nDate: Mon, 20 Dec 2021 14:10:48 GMT\nContent-Type: text/html\nContent-Length: 182\nConnection: close\n\n<html>\n<head><title>200 good Request</title></head>\n<body bgcolor='white'>\n<center><h1>200 good Request</h1></center>\n<hr><center>webcerveau/1.0 (Ubuntu)</center>\n</body>\n</html>\0");
    
    
    //return (full_cmd);

}