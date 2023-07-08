#include "utils/Log.hpp"

#include <string>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <sys/poll.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int opt = -1;
int port = 8080;
sockaddr_in address;
int toContinue = true;
int serverSocket = -1;
int clientSocket = -1;

std::string getHttpResponse(std::string content, int code);

static void handleSignal(int a, siginfo_t *b, void *c)
{
	(void)b;
	(void)c;
	if (a == SIGTERM || a == SIGINT || a == SIGKILL)
	{
		Log::debug << "Recebido um sinal de parada;" << Log::eof;
		toContinue = false;
	}
}
static void init(void)
{
	Log::debug << "init()" << Log::eof;
}
static void loop(void)
{
	Log::debug << "loop()" << Log::eof;

	Log::info << "Iniciando socket na porta " << port << Log::eof;
	if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		Log::fatal << "Erro no socket " << strerror(errno) << port << Log::eof;

	Log::info << "Configurando socket " << serverSocket << " na porta " << port << Log::eof;
	if ((setsockopt(serverSocket, SOL_SOCKET, (SO_REUSEADDR | SO_REUSEPORT), &opt, sizeof(opt))) < 0)
		Log::fatal << "Erro no setsockopt " << strerror(errno) << port << Log::eof;

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	Log::info << "Bind socket " << serverSocket << " na porta " << port << Log::eof;
	if (bind(serverSocket, (struct sockaddr *)&address, sizeof(address)) < 0)
		Log::fatal << "Erro no setsockopt " << strerror(errno) << port << Log::eof;

	Log::info << "Listen socket " << serverSocket << " na porta " << port << Log::eof;
	if (listen(serverSocket, 2) < 0)
		Log::fatal << "Erro no setsockopt " << strerror(errno) << port << Log::eof;

	char buffer[1025] = {0};
	while (toContinue)
	{
		memset(buffer, 0, 1025);

		if (clientSocket == -1)
		{
			Log::info << "Esperando conexão no socket " << serverSocket << " na porta " << port << Log::eof;
			if ((clientSocket = accept(serverSocket, NULL, NULL)) < 0)
				Log::fatal << "Erro no accept " << strerror(errno) << port << Log::eof;
		}

		int countReadBytes = recv(clientSocket, buffer, 1024, 0);

		if (countReadBytes < 0)
		{
			std::cout << "Erro na requisição..." << std::endl;

			std::string res = getHttpResponse("Teste", 500);
			send(clientSocket, res.c_str(), res.size(), 0);

			close(clientSocket);
			clientSocket = -1;
		}
		else if (countReadBytes == 0)
		{
			std::cout << "Fechando a requisição..." << std::endl;

			close(clientSocket);
			clientSocket = -1;
		}
		else
		{
			std::cout << "Requisição: \n"
					  << buffer << std::endl;
			std::cout << "countReadBytes: \n"
					  << countReadBytes << std::endl;

			std::string res = getHttpResponse("", 200);
			send(clientSocket, res.c_str(), res.size(), 0);

			std::cout << "Resposta: \n"
					  << res << std::endl;
			std::cout << "res.size(): \n"
					  << res.size() << std::endl;
		}
	}
}
static void end(void)
{
	Log::debug << "end()" << Log::eof;

	close(serverSocket);
	close(clientSocket);
}

#ifndef TEST_MODE
int main(void)
{
	struct sigaction listenSignal;

	listenSignal.sa_sigaction = handleSignal;
	listenSignal.sa_flags = SA_SIGINFO;
	sigaction(SIGTERM, &listenSignal, NULL);
	sigaction(SIGINT, &listenSignal, NULL);
	sigaction(SIGKILL, &listenSignal, NULL);

	init();
	loop();
	end();
	return (0);
}
#else
#include <gtest/gtest.h>
int main(int argc, char **argv)
{
	// Inicialize o Google Test
	::testing::InitGoogleTest(&argc, argv);

	// Execute os testes
	return RUN_ALL_TESTS();
	return (0);
}
#endif // TEST_MODE
