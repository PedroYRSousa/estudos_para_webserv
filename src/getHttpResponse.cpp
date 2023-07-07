#include <iostream>
#include <string>

std::string getHttpResponse(void)
{
	return "";
}

#ifdef TEST_MODE
#include <gtest/gtest.h>

TEST(MeuTeste, TesteDaFuncao)
{
	EXPECT_TRUE(getHttpResponse() == "");
}
#endif // TEST_MODE
