<div id="top"></div>

# WEBSERV 42

This project is about writing an HTTP server.
You will be able to test it with an actual browser.
HTTP is one of the most used protocols on the internet.
Knowing its arcane will be useful, even if you won’t be working on a website.

## Getting Started ! Linux Only !

### Prerequisites

* Need to install
  ```sh
  sudo apt intall make clang php php-cgi -y
  ```
* Clone repo
  ```sh
  git clone git@github.com:philippebarradas/webserv.git
  cd webserv
  ```
  

!! __Before starting the server don't forget to change the root with your path until /webserv/www/ in the [config_file](https://github.com/philippebarradas/webserv/blob/main/config_file)__ !!

 * Start Server
  ```sh
  Make
  ./webserv config_file
  ```
  
 #### You should see 
 
![disp](https://user-images.githubusercontent.com/52048966/159254512-d298cb20-7b28-4b81-974b-593b0e8992c0.png)

Now you can open your favourite browser and go to :
* [http://localhost:8020/](http://localhost:8020/)
* [http://localhost:8042/](http://localhost:8042/)
* [http://localhost:4242/](http://localhost:4242/)


# webserv
![125_100](https://user-images.githubusercontent.com/52048966/159252369-07c47c3c-c39e-4f4a-8ddc-5b6fdcaf359d.png)
 
HTTP server school project for 42

### Contributor

[@philippebarradas](https://github.com/philippebarradas) |
[@dovdjian](https://github.com/dovdjian) |
[@tsannie](https://github.com/tsannie)

[![forthebadge](https://forthebadge.com/images/badges/made-with-c-plus-plus.svg)](https://forthebadge.com)[![forthebadge](https://forthebadge.com/images/badges/powered-by-coffee.svg)](https://forthebadge.com)

<p align="right">(<a href="#top">back to top</a>)</p>

## SOURCES:

https://www.youtube.com/watch?v=s3o5tixMFho	// video cool\
https://devarea.com/linux-io-multiplexing-select-vs-poll-vs-epoll/ // select vs poll vs epoll\
https://www.csd.uoc.gr/~hy556/material/tutorials/cs556-3rd-tutorial.pdf // powerpoint detaille sur les sockets\
https://www.ibm.com/docs/en/i/7.2?topic=designs-using-poll-instead-select // actual code\
\
https://webdevdesigner.com/q/what-s-the-difference-between-epoll-poll-treatpool-11439/ // epoll poll/select\
https://dev.to/frevib/a-tcp-server-with-kqueue-527 // epoll\
https://www.digitalocean.com/community/tutorials/understanding-nginx-server-and-location-block-selection-algorithms-fr	// block nginx\
https://www.tecmint.com/limit-file-upload-size-in-nginx/	//client_max_body_size

