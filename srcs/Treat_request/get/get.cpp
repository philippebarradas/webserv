/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/20 14:31:46 by user42            #+#    #+#             */
/*   Updated: 2022/02/08 09:55:51 by user42           ###   ########.fr       */
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

int     precondition_vadid(std::string file, std::string date, const Parse_request & parse_head)
{

 {  (void)parse_head;

	//time_t rawtime;
	struct tm * timeinfo;

	char time_modified_file [200];
    std::string time_test = parse_head.get_request("If-Unmodified-Since:");
    //std::string time_test = "Thu, 22 Jan 2022 10:28:47 CET";



            // std::cout << "raw = " << rawtime << std::endl;
    std::string filename = "srcs/Config/default/html_page/404_not_found.html";
    struct stat result;

    if(stat(filename.c_str(), &result)==0)
    {
        //auto mod_time = result.st_ctim; //result.st_mtime;
        //std::cout << result.st_ctim.tv_sec << "=" << result.st_mtim.tv_sec << std::endl;
    }

	//time (&rawtime);
	timeinfo = localtime (&result.st_ctim.tv_sec);

	strftime(time_modified_file, 200, "%a, %d %b %G %T %Z", timeinfo);
	std::string actual_time(time_modified_file);





    struct tm timeinfo_modif;
    struct tm timeinfo_test;

    strptime(time_modified_file , "%a, %d %b %G %T %Z", &timeinfo_modif);
    strptime(time_test.c_str(), "%a, %d %b %G %T %Z", &timeinfo_test);

    std::cout << "\nstrings :\ncompare txt   ["
    << time_test//parse_head.get_request("If-Unmodified-Since:").c_str()
    << "]\nmodified_file [" << time_modified_file << "]"
    << std::endl;

    //std::cout << "time 2 = " << &timeinfo << std::endl;
    //std::cout << "time 3 = " << &timeinfo << std::endl;

    std::cout << "\ntime 2 = " << timeinfo_modif.tm_mday << std::endl;
    std::cout << "time 3 = " << timeinfo_test.tm_mday << std::endl;

    std::cout << "\ntime 2 = " << timeinfo_modif.tm_yday << std::endl;
    std::cout << "time 3 = " << timeinfo_test.tm_yday << std::endl;


    size_t full_time_file = timeinfo_modif.tm_year;
    size_t full_time_test = timeinfo_test.tm_year;


    std::cout << "year = " << timeinfo_modif.tm_year << std::endl;
    std::cout << "year = " << timeinfo_test.tm_year << std::endl;
  std::cout << "day = " << timeinfo_modif.tm_yday << std::endl;
    std::cout << "dat = " << timeinfo_test.tm_yday << std::endl;
      std::cout << "hout = " << timeinfo_modif.tm_hour << std::endl;
    std::cout << "hour = " << timeinfo_test.tm_hour << std::endl;
      std::cout << "min = " << timeinfo_modif.tm_min << std::endl;
    std::cout << "min = " << timeinfo_test.tm_min << std::endl;
      std::cout << "sec = " << timeinfo_modif.tm_sec << std::endl;
    std::cout << "sec = " << timeinfo_test.tm_sec << std::endl;

    std::cout << "final modif = " << full_time_file << std::endl;
    std::cout << "final test = " << full_time_test << std::endl;

    full_time_file = (full_time_test * 1000000000 )
                            + (timeinfo_modif.tm_yday * 1000000)
                            + (timeinfo_modif.tm_hour * 10000)
                            + (timeinfo_modif.tm_min * 100)
                            + (timeinfo_modif.tm_sec);
    full_time_test = (full_time_test * 1000000000)
                            + (timeinfo_test.tm_yday * 1000000)
                            + (timeinfo_test.tm_hour * 10000)
                            + (timeinfo_test.tm_min * 100)
                            + (timeinfo_test.tm_sec);


    std::cout << "final modif = " << full_time_file << std::endl;
    std::cout << "final test  = " << full_time_test << std::endl;
        // if (timeinfo_modif.tm_year > timeinfo_test.tm_year)
    if (full_time_file < full_time_test)
        std::cout << time_modified_file << std::endl;
    else if (full_time_file > full_time_test)
        std::cout << time_test << std::endl;


        }

  return (0);
}

std::string Treat_request::ft_get(std::string full_cmd, const Parse_request & parse_head)//, [loca])
{
    std::string	file = file_to_string("srcs/Config/default/html_page/" + this->act_index, full_cmd);


    //if (precondition_vadid(file,  get_date(), parse_head) == -1)
    //   return (is_precondition_failed(full_cmd));

   std::string filename = "srcs/Config/default/html_page/" + this->act_index;
    struct stat result;

  if (stat(filename.c_str(), &result) == -1)
	{
	    std::cout << "stat failed" << std::endl;
	}

	char time_modified_file [200];
	struct tm * timeinfo;

	timeinfo = localtime (&result.st_ctim.tv_sec);
	strftime(time_modified_file, 200, "%a, %d %b %G %T %Z", timeinfo);
	std::string actual_time(time_modified_file);

    //struct tm timeinfo_modif;

   // strptime(time_modified_file , "%a, %d %b %G %T %Z", &timeinfo_modif);


    std::cout << "DANS LE GET" << std::endl;

    std::cout << "act index = " << this->act_index << std::endl;

    _request_status = "HTTP/1.1 200 OK";
    _server = "webcerveau/1.0";
    _date = "Date: " + get_date();
    if (this->act_index.find(".jpg") != std::string::npos)
      _content_type = "Content-Type: image/jpeg";
    else
      _content_type = "Content-Type: text/html";
    _content_length = "Content-Length: " + int_to_string(file.size()) +
    "\nLast-Modified: " + time_modified_file;
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
