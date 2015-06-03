#include <iostream>
#include <eto/eto.hpp>

static size_t tests = 0;

#define TEST(expression, expected) \
  { \
    auto actual = eto::str(expression); \
    auto pass = (actual == expected); \
    std::cout << (pass? "ok " : "fail ") \
      << ++tests << " - " \
      << #expression << " ==> " << actual; \
    \
    if ( !pass ) \
      std::cout << " != " << expected; \
    \
    std::cout << std::endl; \
  }

int main()
{
  using namespace eto;
  using namespace std;

  TEST(integer(1), "1");
  TEST(integer(-111), "-111");
  TEST(integer("-123456789012345678901234567890"), "-123456789012345678901234567890");

  {
    integer a(1111);
    integer b(2222);
    TEST(a, "1111");
    TEST(b, "2222");
    TEST(a+b, "3333");
    TEST(cons(a,b), "(1111 . 2222)");
  }

  {
    rational a(2,3);
    rational b(2,1);
    TEST(a, "2/3");
    TEST(b, "2");
    TEST(a*b, "4/3");
  }

  TEST(real("1.23456"), "1.23456");
  TEST(eto::string("hello, world!"), "hello, world!");

  {
    integer a(10);
    integer b(20);
    TEST(cons(eto::string("hey")), "(hey)");
    TEST(cons(eto::string("hey"), integer(10)), "(hey . 10)");
    TEST(cons(eto::string("hey"), integer(10)), "(hey . 10)");
    TEST(cons(eto::string("hey"), a), "(hey . 10)");
    TEST(cons(a, integer(20)), "(10 . 20)");
    TEST(cons(a, b), "(10 . 20)");
    TEST(cons(a*a, b), "(100 . 20)");
    TEST(cons(a*a, a+b), "(100 . 30)");
  }

  cout << "1.." << tests << endl;
}
