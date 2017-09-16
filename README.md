# Format <a target="_blank" href="https://travis-ci.org/napiknikkpek/format">![Travis status][badge.Travis]</a>

Simple constexpr format function.

1. Using constexpr metaprogramming instead of template metaprogramming make compilation time much smaller.
1. It uses python's replacement fields with optional argument index - `{d*}`.

Usage:
```cpp
#include <format.hpp>
#include <boost/hana/string.hpp>

using namespace boost::hana::literals;

std::cout << format("string: {1}, number: {0}"_s, 1, "asdf");
```



<!-- Links -->
[badge.Travis]: https://travis-ci.org/napiknikkpek/format.svg?branch=master
