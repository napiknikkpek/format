#include <iostream>

#include <boost/hana/string.hpp>

#include "format.hpp"

using namespace boost::hana::literals;

int main() {
  std::cout << format("string: {}, number: {}"_s, "asdf", 100) << std::endl;
  std::cout << format("number: {1}, string: {0}"_s, "asdf", 100) << std::endl;
  return 0;
}
