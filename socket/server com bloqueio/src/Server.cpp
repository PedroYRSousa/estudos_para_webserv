#include "Server.hpp"

Server::Server() : opt(1), valread(-1), serverFd(-1), new_socket(-1)
{
	this->init();
}

Server::~Server()
{
}

Server *Server::instance = nullptr;

Server *Server::getInstance()
{
	if (instance == nullptr)
		instance = new Server();
	return instance;
}

void Server::init()
{
	int *opt = &this->opt;
	int sizeOpt = sizeof(*opt);
	int *serverFd = &this->serverFd;
	struct sockaddr_in *address = &this->address;
	int addrlen = sizeof(*address);

	// Inicia o FD do servidor
	if ((*serverFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cout << "socket error: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	// Força o FD do socket na PORTA
	if (setsockopt(*serverFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, opt, sizeOpt))
	{
		std::cout << "setsockopt error: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
	this->address.sin_family = AF_INET;
	this->address.sin_addr.s_addr = INADDR_ANY;
	this->address.sin_port = htons(PORT);

	// Força o FD do socket na PORTA
	if (bind(*serverFd, (struct sockaddr *)address, addrlen) < 0)
	{
		std::cout << "bind error: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
}

void Server::loop()
{
	int serverFd = this->serverFd;
	struct sockaddr_in *address = &this->address;
	int addrlen = sizeof(*address);

	if (listen(serverFd, 3) < 0)
	{
		std::cout << "listen error: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	if ((new_socket = accept(serverFd, (struct sockaddr *)address, (socklen_t *)&addrlen)) < 0)
	{
		std::cout << "accept error: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	// Simula uma operação difícil de realizar
	sleep(10);

	read(new_socket, buffer, 1024);
	printf("%s\n", buffer);
	send(new_socket, hello, strlen(hello), 0);
	printf("Hello message sent\n");

	// closing the connected socket
	close(new_socket);
}

void Server::stop()
{
	shutdown(this->serverFd, SHUT_RDWR);
}
