# webserv
webserv

Savoir si on a brew ou non:

brew --version

Install Homebrew:

mkdir homebrew && curl -L https://github.com/Homebrew/brew/tarball/master | tar xz --strip 1 -C homebrew
export PATH=$HOME/homebrew/bin:$PATH

Install telnet:

brew install telnet

Install nginx:

brew install nginx

SOURCES:

https://www.youtube.com/watch?v=s3o5tixMFho	// video cool
https://devarea.com/linux-io-multiplexing-select-vs-poll-vs-epoll/ // select vs poll vs epoll
https://www.csd.uoc.gr/~hy556/material/tutorials/cs556-3rd-tutorial.pdf // powerpoint detaille sur les sockets
https://www.ibm.com/docs/en/i/7.2?topic=designs-using-poll-instead-select // actual code

To stop nginx:

ps -lef | grep -i nginx: | awk '{ print $2}' | xargs kill -9
