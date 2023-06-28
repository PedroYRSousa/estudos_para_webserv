#include <unistd.h>

#include "../test/catch.hpp"

int sum(int a, int b)
{
  return a + b;
}

// Estrutura
// [ nome do teste ] [ tags ]
// O nome do teste será exibido no console, as tags não.
// A tag não pode ser vazia.
TEST_CASE("Testa a função sum", "[sum]")
{
  // Se ele quebrar aqui, não vai funcionar e não vai continuar os testes
  // REQUIRE(sum(1, 2) == 2);

  // O Section separa em escopos os testes, caso algum teste do section quebre
  // Apenas os testes posteriores desse que quebrou dentro de section irão
  // deixar de serem executados. Os outros sections serão executados.
  SECTION("Teste A")
  {
    int a = 1;
    int b = 2;
    REQUIRE(sum(a, b) == 4);
  };

  SECTION("Teste B")
  {
    int a = 2;
    int b = 3;
    REQUIRE(sum(a, b) == 2);
  };

  // Aqui ele faz um benchmark da função
  BENCHMARK("Teste D")
  {
    int a = 6;
    int b = 7;
    return sum(a, b);
  };

  // Se quebrar aqui o teste 'c' não sera feito
  // REQUIRE(sum(11, 22) == 6);

  SECTION("Teste C")
  {
    int a = 4;
    int b = 5;
    REQUIRE(sum(a, b) == 4);
  };
}
