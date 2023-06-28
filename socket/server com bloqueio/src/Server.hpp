#ifndef SERVER_HPP
#define SERVER_HPP

#ifndef PORT
#define PORT 8080
#endif

#include <iostream>
#include <string>

// https://man7.org/linux/man-pages/man2/listen.2.html
// listen, shutdown, send, bind, accept
#include <sys/socket.h>

// https://man7.org/linux/man-pages/man3/errno.3.html
// errno
#include <errno.h>

// https://man7.org/linux/man-pages/man3/strerror.3.html
// strerror
#include <string.h>

// https://man7.org/linux/man-pages/man7/ip.7.html
// struct sockaddr_in
#include <netinet/in.h>

// read, close
#include <unistd.h>

// signal, kill, struct sigaction
#include <signal.h>

// waitpid
#include <sys/types.h>
#include <sys/wait.h>

class Server
{
public:
	~Server();

	static Server *getInstance();

	void loop();
	void stop();

private:
	Server();
	void init();

	static Server *instance;

	int opt;
	int valread;
	int serverFd;
	int new_socket;
	char buffer[1024];
	const char *hello = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 12\r\n\r\nHello World!";
	struct sockaddr_in address;
};

#endif
