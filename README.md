# Format <a target="_blank" href="https://travis-ci.org/napiknikkpek/format">![Travis status][badge.Travis]</a>

Simple c++17 compile time formatting implemented with constexpr.\
Use python format syntax.

Usage:
```cpp
#include <format.hpp>
#include <boost/hana/string.hpp>

using namespace boost::hana::literals;

std::string str = format("string: {1}, number: {0}"_s, 1, "asdf");
```
```cpp
#include <format.hpp>

std::string str = FORMAT("string: {1}, number: {0}")(1, "asdf");
```



<!-- Links -->
[badge.Travis]: https://travis-ci.org/napiknikkpek/format.svg?branch=master
