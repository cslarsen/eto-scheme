#ifndef INC_ETO_HPP_20150603
#define INC_ETO_HPP_20150603

#include <sstream>
#include <utility>
#include <iostream>
#include <cstdint>
#include <typeinfo>

#include <gmpxx.h>

#define __ETO_DECLARE_COMPOUND_OPERATOR(__class, __member, __operator) \
  __class& operator __operator (const __class& p) \
  { \
    __member __operator p.__member; \
    return *this; \
  }

#define __ETO_DECLARE_OPERATOR(__class, __member, __operator) \
  friend __class operator __operator (const __class& l, const __class& r) \
  { \
    return __class (l.__member __operator r.__member); \
  }

#define __ETO_DECLARE_FIX_OPERATOR(__class, __member, __operator) \
  __class& operator __operator() \
  { \
    __operator __member; \
    return *this; \
  } \
  \
  __class operator __operator(int) \
  { \
    __class before = __class(*this); \
    __operator __member; \
    return before; \
  }

namespace eto {

// TODO: new/delete for gc
class object {
public:
  object()
  {
  }

  virtual std::ostream& print(std::ostream& s) const
  {
    s << "#<object " << this << ">";
    return s;
  }

  virtual ~object()
  {
  }
};

class integer : public object {
  mpz_class z;
public:
  template<typename X>
  integer(X x) : z(x)
  {
  }

  friend std::ostream& operator<<(std::ostream& s, const integer& i)
  {
    return s << i.z;
  }

  virtual std::ostream& print(std::ostream& s) const
  {
    return s << *this;
  }

  __ETO_DECLARE_OPERATOR(integer, z, +)
  __ETO_DECLARE_OPERATOR(integer, z, -)
  __ETO_DECLARE_OPERATOR(integer, z, *)
  __ETO_DECLARE_OPERATOR(integer, z, /)
  __ETO_DECLARE_OPERATOR(integer, z, %)
  __ETO_DECLARE_OPERATOR(integer, z, &)
  __ETO_DECLARE_OPERATOR(integer, z, |)
  __ETO_DECLARE_OPERATOR(integer, z, ^)

  __ETO_DECLARE_COMPOUND_OPERATOR(integer, z, +=)
  __ETO_DECLARE_COMPOUND_OPERATOR(integer, z, -=)
  __ETO_DECLARE_COMPOUND_OPERATOR(integer, z, *=)
  __ETO_DECLARE_COMPOUND_OPERATOR(integer, z, /=)
  __ETO_DECLARE_COMPOUND_OPERATOR(integer, z, %=)
  __ETO_DECLARE_COMPOUND_OPERATOR(integer, z, &=)
  __ETO_DECLARE_COMPOUND_OPERATOR(integer, z, |=)
  __ETO_DECLARE_COMPOUND_OPERATOR(integer, z, ^=)

  __ETO_DECLARE_FIX_OPERATOR(integer, z, ++)
  __ETO_DECLARE_FIX_OPERATOR(integer, z, --)
};

class rational : public object {
  mpq_class q;
public:
  template<typename X>
  rational(X x) : q(x)
  {
  }

  template<typename X, typename Y>
  rational(X x, Y y) : q(x, y)
  {
  }

  friend std::ostream& operator<<(std::ostream& s, const rational& i)
  {
    return s << i.q;
  }

  virtual std::ostream& print(std::ostream& s) const
  {
    return s << *this;
  }

  __ETO_DECLARE_OPERATOR(rational, q, +)
  __ETO_DECLARE_OPERATOR(rational, q, -)
  __ETO_DECLARE_OPERATOR(rational, q, *)
  __ETO_DECLARE_OPERATOR(rational, q, /)

  __ETO_DECLARE_COMPOUND_OPERATOR(rational, q, +=)
  __ETO_DECLARE_COMPOUND_OPERATOR(rational, q, -=)
  __ETO_DECLARE_COMPOUND_OPERATOR(rational, q, *=)
  __ETO_DECLARE_COMPOUND_OPERATOR(rational, q, /=)

  __ETO_DECLARE_FIX_OPERATOR(rational, q, ++)
  __ETO_DECLARE_FIX_OPERATOR(rational, q, --)
};

class real : public object {
  mpf_class f;
public:
  template<typename X>
  real(X x) : f(x)
  {
  }

  friend std::ostream& operator<<(std::ostream& s, const real& r)
  {
    return s << r.f;
  }

  virtual std::ostream& print(std::ostream& s) const
  {
    return s << *this;
  }

  __ETO_DECLARE_OPERATOR(real, f, +)
  __ETO_DECLARE_OPERATOR(real, f, -)
  __ETO_DECLARE_OPERATOR(real, f, *)
  __ETO_DECLARE_OPERATOR(real, f, /)

  __ETO_DECLARE_COMPOUND_OPERATOR(real, f, +=)
  __ETO_DECLARE_COMPOUND_OPERATOR(real, f, -=)
  __ETO_DECLARE_COMPOUND_OPERATOR(real, f, *=)
  __ETO_DECLARE_COMPOUND_OPERATOR(real, f, /=)

  __ETO_DECLARE_FIX_OPERATOR(real, f, ++)
  __ETO_DECLARE_FIX_OPERATOR(real, f, --)
};

// TODO: utf-8
class string : public std::string, public object {
public:
  template<typename X>
  string(X x) : std::string(x)
  {
  }

  virtual std::ostream& print(std::ostream& s) const
  {
    return s << static_cast<const std::string&>(*this);
  }
};

class cons : public object {
public:
  object* car;
  object* cdr;

  cons(object* car_, object* cdr_ = nullptr):
    car(car_),
    cdr(cdr_)
  {
  }

  cons(object& car_, object& cdr_):
    cons(&car_, &cdr_)
  {
  }

  cons(object&& car_):
    cons(&car_)
  {
  }

  cons(object&& car_, object&& cdr_):
    cons(&car_, &cdr_)
  {
  }

  cons(object&& car_, object& cdr_):
    cons(&car_, &cdr_)
  {
  }

  cons(object& car_, object&& cdr_):
    cons(&car_, &cdr_)
  {
  }

  virtual std::ostream& print(std::ostream& s) const
  {
    s << "(";

    if ( car )
      car->print(s);
    else
      s << "#<nil>";

    if ( cdr ) {
      s << " . ";
      cdr->print(s);
    }

    s << ")";
    return s;
  }

  friend std::ostream& operator<<(std::ostream& s, const eto::cons& o)
  {
    return o.print(s);
  }
};

std::string demangled(const char* s);

template<typename X>
std::string type_name(const X& x)
{
  return demangled(typeid(x).name());
}

std::string str(const object& o);

} // namespace eto

#endif
