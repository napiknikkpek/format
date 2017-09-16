#include <iostream>

#include <boost/hana/string.hpp>

#include "format.hpp"

using namespace boost::hana::literals;

int main() {
  std::cout << format("string: {}, number: {}"_s, "asdf", 100) << std::endl;
  std::cout << format("string: {1}, number: {0}"_s, 100, "asdf") << std::endl;
  return 0;
}
