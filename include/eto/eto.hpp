#ifndef INC_ETO_HPP_20150603
#define INC_ETO_HPP_20150603

#include <cstdint>
#include <iostream>
#include <sstream>
#include <typeinfo>
#include <utility>
#include <vector>

/*
 * TODO:
 * - Printing blows up the call stack for lists. Fix that with a loop.
 * - rename virtual print() to something else, e.g. stream, to_s?
 * - see if we can avoid const-casting, I don't like it
 * - try to reduce number of pair-constructors, don't like to have too many of
 *   them
 * - look more into move semantics
 * - try to remove var; it's another boxing layer that we don't really should
 *   need (object should be enough). Is it possible to object to create the
 *   various types? i.e., is it possible for a base class to instantiate itself
 *   as one of the children types without using static methods?
 */

#include <gmpxx.h>

#define __ETO_DECLARE_COMPOUND_OPERATOR(__class, __member, __operator) \
  inline __class& operator __operator (const __class& p) \
  { \
    __member __operator p.__member; \
    return *this; \
  }

#define __ETO_DECLARE_OPERATOR(__class, __member, __operator) \
  inline friend __class operator __operator (const __class& l, \
                                             const __class& r) \
  { \
    return __class (l.__member __operator r.__member); \
  }

#define __ETO_DECLARE_ADFIX_OPERATOR(__class, __member, __operator) \
  inline __class& operator __operator() \
  { \
    __operator __member; \
    return *this; \
  } \
  \
  inline __class operator __operator(int) \
  { \
    __class before = __class(*this); \
    __operator __member; \
    return before; \
  }

namespace eto {

// TODO: new/delete for gc
class object {
public:
  virtual std::ostream& print(std::ostream& s) const = 0;
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

  __ETO_DECLARE_ADFIX_OPERATOR(integer, z, ++)
  __ETO_DECLARE_ADFIX_OPERATOR(integer, z, --)
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

  __ETO_DECLARE_ADFIX_OPERATOR(rational, q, ++)
  __ETO_DECLARE_ADFIX_OPERATOR(rational, q, --)
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

  __ETO_DECLARE_ADFIX_OPERATOR(real, f, ++)
  __ETO_DECLARE_ADFIX_OPERATOR(real, f, --)
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
    return s << "\"" << static_cast<const std::string&>(*this) << "\"";
  }
};

class pair : public object {
public:
  object* car;
  object* cdr;

  pair(object* car_, object* cdr_ = nullptr):
    car(car_),
    cdr(cdr_)
  {
  }

  pair(object& car_, object& cdr_):
    pair(&car_, &cdr_)
  {
  }

  pair(object&& car_):
    pair(&car_)
  {
  }

  pair(object&& car_, object&& cdr_):
    pair(&car_, &cdr_)
  {
  }

  pair(object&& car_, object& cdr_):
    pair(&car_, &cdr_)
  {
  }

  pair(object& car_, object&& cdr_):
    pair(&car_, &cdr_)
  {
  }

  virtual std::ostream& print(std::ostream& s, bool parens) const
  {
    if ( parens )
      s << "(";

    if ( car )
      car->print(s);
    else
      s << "#<nil>";

    if ( cdr ) {
      // if cdr is pair, pass param parens=false
      eto::pair* p = dynamic_cast<eto::pair*>(cdr);
      if ( p ) {
        s << " ";
        p->print(s, false);
      } else {
        s << " . ";
        cdr->print(s);
      }
    }

    if ( parens )
      s << ")";

    return s;
  }

  virtual std::ostream& print(std::ostream& s) const
  {
    return print(s, true);
  }

  friend std::ostream& operator<<(std::ostream& s, const eto::pair& o)
  {
    return o.print(s);
  }
};

class var {
  object* p;
public:
  var(const long& v):
    p(new integer(v))
  {
  }

  var(const char* v):
    p(new eto::string(v))
  {
  }

  object* ptr()
  {
    return p;
  }

  static var real(const double& v)
  {
    return var(new eto::real(v));
  }

  static var pair(var&& car, var&& cdr)
  {
    return var(new eto::pair(car.p, cdr.p));
  }

  static var pair(var&& car)
  {
    return var(new eto::pair(car.p));
  }

  var(const var& v):
    p(v.p)
  {
  }

  var(const object* p):
    p(const_cast<object*>(p))
  {
  }

  var(const long& dividend, const long& divisor):
    p(new rational(dividend, divisor))
  {
  }

  var(const std::vector<var>& v);

  operator object&()
  {
    return *p;
  }

  friend std::ostream& operator<<(std::ostream& s, const var& v)
  {
    return v.p->print(s);
  }

  friend var cons(const var&, const var&);
  friend var cons(const var&);
};

class vector : public object {
  std::vector<object*> v;
public:
  vector(const std::vector<var>& v_)
  {
    for ( auto i : v_ )
      v.push_back(const_cast<object*>(i.ptr()));
  }

  virtual std::ostream& print(std::ostream& s) const
  {
    s << "#(";

    bool f = true;
    for ( auto i : v ) {
      if ( f )
        f = false;
      else
        s << " ";
      i->print(s);
    }

    s << ")";
    return s;
  }
};


std::string demangled(const char* s);

template<typename X>
std::string type_name(const X& x)
{
  return demangled(typeid(x).name());
}

var cons(const var&, const var&);
var cons(const var&);

std::string str(const object&);
std::string str(const var&);

} // namespace eto

#endif
