eto scheme
==========

Implements R7RS small.

Requirements
------------

  * A C++11/C++14 compiler
  * GNU Make
  * GMP with gmpxx.h, https://gmplib.org

Building
--------

You may have to tweak some parameters. E.g. you may want to use
`CXXFLAGS := ... -std=c++1y` for clang 3.4 or earlier.
You may also want to set `CXX` to a compiler other than the default.

License
------
Copyright (C) 2015 Christian Stigen Larsen  
Distributed under the LGPL v2 or later.
