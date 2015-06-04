#include <eto/eto.hpp>

#ifdef __GNUG__
#include <cxxabi.h>
#endif

namespace eto {

#ifndef __GNUG__
std::string demangled(const char* s)
{
  return s;
}
#else
std::string demangled(const char* s)
{
  int status;
  auto d = abi::__cxa_demangle(s, NULL, NULL, &status);
  auto r = status==0? std::string(d) : s;
  free(d);
  return r;
}
#endif

std::string str(const object& o)
{
  std::ostringstream s;
  o.print(s);
  return s.str();
}

std::string str(const var& o)
{
  std::ostringstream s;
  s << o;
  return s.str();
}

var cons(const var& car, const var& cdr)
{
  return var::pair(var(car.p), var(cdr.p));
}

var cons(const var& car)
{
  return var::pair(var(car.p));
}

} // namespace eto
