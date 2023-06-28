#include <iostream>
#include "Server.hpp"

static void handle_signal(int sig)
{
  Server *server = Server::getInstance();

  if (sig == SIGTERM || sig == SIGINT || sig == SIGKILL || sig == SIGQUIT)
  {
    std::cout << "Parando o servidor..." << std::endl;
    server->stop();
  }
}

// Sequencia
// socket -> setsockopt -> bind -> listen -> accept -> send -> close
// https://www.geeksforgeeks.org/socket-programming-cc/
int main(void)
{
  Server *server = Server::getInstance();

  signal(SIGTERM, handle_signal);
  signal(SIGINT, handle_signal);
  signal(SIGKILL, handle_signal);
  signal(SIGQUIT, handle_signal);

  while (true)
    server->loop();

  server->stop();

  return 0;
}
