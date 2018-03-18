#include <iostream>

#include <boost/hana/string.hpp>
#include <format.hpp>

int main() {
  using namespace boost::hana::literals;

  std::cout << format("string: {}, number: {}"_s, "asdf", 100) << std::endl;
  std::cout << format("string: {1}, number: {0}, the end"_s, 100, "asdf")
            << std::endl;

  std::cout << FORMAT("string: {}, number: {}")("asdf", 100) << std::endl;
  std::cout << FORMAT("string: {1}, number: {0}, the end")(100, "asdf")
            << std::endl;

  return 0;
}
