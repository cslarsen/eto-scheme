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

} // namespace eto
