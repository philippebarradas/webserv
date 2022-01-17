/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodjian <dovdjianpro@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/20 14:31:46 by user42            #+#    #+#             */
/*   Updated: 2022/01/17 17:30:00 by dodjian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../method.hpp"
#include <vector>

std::string Method::ft_get(std::string full_cmd)//, [loca])
{
    std::cout << std::endl << PURPLE2 << "DANS LE GET" << std::endl << std::endl << END;
    std::cout << "act index = " << this->act_index << std::endl;
    std::string	file = file_to_string("srcs/Config/default/html_page/" + this->act_index, full_cmd);

    _request_status = "HTTP/1.1 200 OK";
    _server = "webcerveau/1.0";
    _date = get_date();
    _content_type = "Content-Type: text/html";
    _content_length = "Content-Length: " + static_cast<std::ostringstream*>( &(std::ostringstream() << file.size()) )->str();
    _connection = "Connection: keep-alive";

    /*
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
    std::cout << std::endl << PURPLE2 << "FIN DU GET" << std::endl << std::endl << END;
    return (build_header(full_cmd) + file);

    //return (full_cmd);

}
