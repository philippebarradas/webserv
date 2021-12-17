#include "../method.hpp"
#include <vector>

void Method::ft_get(std::string full_cmd)//, [loca])
{
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
    std::cout << "server_protocol = [" << server_protocol << "]" << std::endl;

    //path.


    //std::string http = NULL;    

    //std::cout << "path = [" << path << "]" << std::endl;   
   // std::cout << "http = [" << http << "]" << std::endl;   
   // std::cout << "Full_cmd = [" << full_cmd << "]" << std::endl;   
}