# Format <a target="_blank" href="https://travis-ci.org/napiknikkpek/format">![Travis status][badge.Travis]</a>

Simple c++17 compile time formatting implemented with constexpr.

1. Using constexpr functions instead of recursive templates make compilation time much smaller
1. It uses python's replacement fields with optional argument index - `{\d*}`.

Usage:
```cpp
#include <format.hpp>

std::cout << FORMAT("string: {1}, number: {0}")(1, "asdf");
```



<!-- Links -->
[badge.Travis]: https://travis-ci.org/napiknikkpek/format.svg?branch=master
