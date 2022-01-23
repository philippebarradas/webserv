/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/20 14:31:46 by user42            #+#    #+#             */
/*   Updated: 2022/01/23 18:09:00 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../treat_request.hpp"
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>

/* time_t rawtimet;
                                    struct tm * timeinfot;
                                    char time_test [200];

                                    time (&rawtimet);
                                    timeinfot = localtime (&rawtimet);

                                    strftime(time_test, 200, "%a, %d %b %G %T %Z",timeinfot); */

int     precondition_vadid(std::string file, std::string date, const Parse_header & parse_head)
{

   {  (void)parse_head;
    
	time_t rawtime;
	struct tm * timeinfo;

	char time_real [200];
    std::string time_test  = "Thu, 22 Jan 2022 10:28:47 CET";



   // std::cout << "raw = " << rawtime << std::endl;

	time (&rawtime);
	timeinfo = localtime (&rawtime);

	strftime(time_real, 200, "%a, %d %b %G %T %Z",timeinfo);
	std::string actual_time(time_real);
	

    

    struct tm timeinfo_real;
    struct tm timeinfo_test;

    strptime(time_real , "%a, %d %b %G %T %Z", &timeinfo_real);
    strptime(time_test.c_str(), "%a, %d %b %G %T %Z", &timeinfo_test);

    std::cout << "\nstrings :\nwanted["
    << time_test//parse_head.get_request("If-Unmodified-Since:").c_str()
    << "]\nread  [" << time_real << "]"
    << std::endl;

    //std::cout << "time 2 = " << &timeinfo << std::endl;
    //std::cout << "time 3 = " << &timeinfo << std::endl;

    std::cout << "\ntime 2 = " << timeinfo_real.tm_mday << std::endl;
            std::cout << "time 3 = " << timeinfo_test.tm_mday << std::endl;

        // if (timeinfo_real.tm_year > timeinfo_test.tm_year)

            if (&timeinfo_real > &timeinfo_test)
                std::cout << "\nreal time > time wanted" << std::endl;
            else if (&timeinfo_real == &timeinfo_test)
                std::cout << "\nreal time == time wanted" << std::endl;
            else
                std::cout << "\nreal time < time wanted" << std::endl;

        } 

            {   
                time_t rawtime;
                struct tm timeinfo;

            // memset(&timeinfo, 0, sizeof(struct tm));

                char time_real [200];

            
            //strptime("Thu, 20 Jan 2022 10:31:22 CET", "%a, %d %b %G %T %Z", &timeinfo);
                strptime(parse_head.get_request("If-Unmodified-Since:").c_str(), "%a, %d %b %G %T %Z", &timeinfo);


                struct tm * timeinfo_real;
                
        //time (&rawtime);

        rawtime = (time_t)&timeinfo;
        timeinfo_real = (tm *)(&rawtime);
        
       // timeinfo->tm_mon = timeinfo->tm_mon -1;
        std::cout << "\n\ntime 2 = " << &timeinfo << std::endl;
        strftime(time_real, 200, "%a, %d %b %G %T %Z", timeinfo_real);
        
        std::string actual_time(time_real);
        std::cout << "time 2 = " << actual_time << std::endl;

	
        //rawtime = mktime(&timeinfo);
        //printf("2= %d \n", (int)rawtime); //unix time-stamp
        //printf("2 = %s \n", ctime(&rawtime)); //human readable date 
        
    } 
    //std::cout << "time info = ["<< timeinfo << "]"<< std::endl;


    //std::cout << "["<< actual_time << "] vs \n[" << parse_head.get_request("If-Unmodified-Since:") << "]" << std::endl;
    return (0);
}

std::string Treat_request::ft_get(std::string full_cmd, const Parse_header & parse_head)//, [loca])
{
    std::string	file = file_to_string("srcs/Config/default/html_page/" + this->act_index, full_cmd);


    if (precondition_vadid(file,  get_date(), parse_head) == -1)
        return (is_precondition_failed(full_cmd));

    std::cout << "DANS LE GET" << std::endl;

    std::cout << "act index = " << this->act_index << std::endl;

    _request_status = "HTTP/1.1 200 OK";
    _server = "webcerveau/1.0";
    _date = get_date();
    _content_type = "Content-Type: text/html";
    _content_length = "Content-Length: " + int_to_string(file.size());
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
    return (build_header(full_cmd) + file);  
    
    //return (full_cmd);

}