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
    TEST(eto::pair(a,b), "(1111 . 2222)");
  }

  {
    rational a(2,3);
    rational b(2,1);
    TEST(a, "2/3");
    TEST(b, "2");
    TEST(a*b, "4/3");
  }

  TEST(real("1.23456"), "1.23456");
  TEST(eto::string("hello, world!"), "\"hello, world!\"");

  {
    integer a(10);
    integer b(20);
    TEST(eto::pair(eto::string("hey")), "(\"hey\")");
    TEST(eto::pair(eto::string("hey"), integer(10)), "(\"hey\" . 10)");
    TEST(eto::pair(eto::string("hey"), integer(10)), "(\"hey\" . 10)");
    TEST(eto::pair(eto::string("hey"), a), "(\"hey\" . 10)");
    TEST(eto::pair(a, integer(20)), "(10 . 20)");
    TEST(eto::pair(a, b), "(10 . 20)");
    TEST(eto::pair(a*a, b), "(100 . 20)");
    TEST(eto::pair(a*a, a+b), "(100 . 30)");
  }

  TEST(eto::string(type_name(integer(100)+integer(200))), "\"eto::integer\"");
  TEST(eto::string(type_name(real(100)+real(200))), "\"eto::real\"");
  TEST(eto::string(type_name(rational(100,2)+rational(200,2))), "\"eto::rational\"");
  TEST(eto::string(type_name(eto::string("hello"))), "\"eto::string\"");

  TEST(var(123), "123");
  TEST(var("hello"), "\"hello\"");

  TEST(cons(123, "hey"), "(123 . \"hey\")");
  TEST(cons(1, cons(2, 3)), "(1 2 . 3)");
  TEST(cons(1, cons(cons(2, 3), 4)), "(1 (2 . 3) . 4)");
  TEST(cons("foo", cons(cons("bar", 3), "baz")), "(\"foo\" (\"bar\" . 3) . \"baz\")");
  TEST(cons(1, cons(2, cons(3))), "(1 2 3)");

  cout << "1.." << tests << endl;
}
