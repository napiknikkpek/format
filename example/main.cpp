#include <iostream>

#include "format.hpp"

int main() {
  std::cout << FORMAT("string: {}, number: {}")("asdf", 100) << std::endl;
  std::cout << FORMAT("string: {1}, number: {0}")(100, "asdf") << std::endl;
  return 0;
}
