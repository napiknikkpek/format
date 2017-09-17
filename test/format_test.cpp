#define BOOST_TEST_MODULE format
#include <boost/test/included/unit_test.hpp>

#include "format.hpp"

#include <boost/hana/string.hpp>

using namespace boost::hana::literals;

BOOST_AUTO_TEST_CASE(empty) {
  BOOST_CHECK_EQUAL("", format(""_s));
  BOOST_CHECK_EQUAL("asdf", format("asdf"_s));
}

BOOST_AUTO_TEST_CASE(boundary) {
  BOOST_CHECK_EQUAL(" 1", format(" {}"_s, 1));
  BOOST_CHECK_EQUAL("1 ", format("{} "_s, 1));
  BOOST_CHECK_EQUAL("1", format("{}"_s, 1));
}

BOOST_AUTO_TEST_CASE(escaped) {
  BOOST_CHECK_EQUAL("{", format("{{"_s, 0));
  BOOST_CHECK_EQUAL("}", format("}}"_s, 0));
  BOOST_CHECK_EQUAL("{}", format("{{}}"_s, 0));
  BOOST_CHECK_EQUAL("{0}", format("{{{}}}"_s, 0));
  BOOST_CHECK_EQUAL("{{0", format("{{{{{}"_s, 0));
  BOOST_CHECK_EQUAL("0}}", format("{}}}}}"_s, 0));
}

BOOST_AUTO_TEST_CASE(empty_spec) {
  BOOST_CHECK_EQUAL("0 1 2", format("{} {} {}"_s, 0, 1, 2));
}

BOOST_AUTO_TEST_CASE(numeric_spec) {
  BOOST_CHECK_EQUAL("0", format("{0}"_s, 0));
  BOOST_CHECK_EQUAL("0 1 2", format("{0} {1} {2}"_s, 0, 1, 2));
  BOOST_CHECK_EQUAL("2 1 0", format("{2} {1} {0}"_s, 0, 1, 2));
  BOOST_CHECK_EQUAL("10", format("{10}"_s, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10));
}

BOOST_AUTO_TEST_CASE(various) {
  BOOST_CHECK_EQUAL("asdf 1 1", format("{} {} {}"_s, "asdf", 1.0, true));
}
