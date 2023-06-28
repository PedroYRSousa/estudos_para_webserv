#include <iostream>

#define PORT 8080

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

// Sequencia
// socket -> setsockopt -> bind -> listen -> accept -> send -> close
// https://www.geeksforgeeks.org/socket-programming-cc/
int main(void)
{
  int server_fd, new_socket, valread;
  int opt = 1;
  struct sockaddr_in address;
  int addrlen = sizeof(address);
  char buffer[1024] = {0};
  const char *hello = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 12\r\n\r\nHello World!";

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    std::cout << "socket error: " << strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
  }

  // Forcefully attaching socket to the port 8080
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
  {
    std::cout << "setsockopt error: " << strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  // Forcefully attaching socket to the port 8080
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
  {
    std::cout << "bind error: " << strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, 3) < 0)
  {
    std::cout << "listen error: " << strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
  }

  if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
  {
    std::cout << "accept error: " << strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
  }

  valread = read(new_socket, buffer, 1024);
  (void)valread;
  printf("%s\n", buffer);
  send(new_socket, hello, strlen(hello), 0);
  printf("Hello message sent\n");

  // closing the connected socket
  close(new_socket);
  // closing the listening socket
  shutdown(server_fd, SHUT_RDWR);

  return 0;
}
