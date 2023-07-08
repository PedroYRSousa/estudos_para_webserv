#include <map>
#include <string>
#include <sstream>
#include <iostream>

std::string getHttpResponse(std::string content, int code)
{
	std::map<int, std::string> reasonByCode;
	reasonByCode.insert(std::make_pair(100, "Continue"));
	reasonByCode.insert(std::make_pair(200, "OK"));
	reasonByCode.insert(std::make_pair(204, "No Content"));
	reasonByCode.insert(std::make_pair(400, "Bad Request"));
	reasonByCode.insert(std::make_pair(404, "Not Found"));
	reasonByCode.insert(std::make_pair(500, "Internal Server Error"));

	std::stringstream response;
	response << "HTTP/1.1 " << code << " " << reasonByCode[code] << "\r\n";
	response << "Connection: close\r\n";
	response << "Content-Length: " << content.size() << "\r\n";

	response << "\r\n"
			 << content;
	return response.str();
}

#ifdef TEST_MODE
#include <gtest/gtest.h>
TEST(TestandoGetHttpResponse, TesteCom1xx)
{
	std::string content = "Teste";
	EXPECT_TRUE(getHttpResponse(content, 100) == "HTTP/1.1 100 Continue\r\nContent-Length: 5\r\n\r\nTeste");
}
TEST(TestandoGetHttpResponse, TesteCom2xx)
{
	std::string content = "Teste";
	EXPECT_TRUE(getHttpResponse(content, 200) == "HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nTeste");
}
TEST(TestandoGetHttpResponse, TesteCom4xx)
{
	std::string content = "Teste";
	EXPECT_TRUE(getHttpResponse(content, 400) == "HTTP/1.1 400 Bad Request\r\nContent-Length: 5\r\n\r\nTeste");
	EXPECT_TRUE(getHttpResponse(content, 404) == "HTTP/1.1 404 Not Found\r\nContent-Length: 5\r\n\r\nTeste");
}
#endif // TEST_MODE
