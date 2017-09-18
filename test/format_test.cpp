#define BOOST_TEST_MODULE format
#include <boost/test/included/unit_test.hpp>

#include "format.hpp"

BOOST_AUTO_TEST_CASE(empty) {
  BOOST_CHECK_EQUAL("", FORMAT("")());
  BOOST_CHECK_EQUAL("asdf", FORMAT("asdf")());
}

BOOST_AUTO_TEST_CASE(boundary) {
  BOOST_CHECK_EQUAL(" 1", FORMAT(" {}")(1));
  BOOST_CHECK_EQUAL("1 ", FORMAT("{} ")(1));
  BOOST_CHECK_EQUAL("1", FORMAT("{}")(1));
}

BOOST_AUTO_TEST_CASE(escaped) {
  BOOST_CHECK_EQUAL("{", FORMAT("{{")(0));
  BOOST_CHECK_EQUAL("}", FORMAT("}}")(0));
  BOOST_CHECK_EQUAL("{}", FORMAT("{{}}")(0));
  BOOST_CHECK_EQUAL("{0}", FORMAT("{{{}}}")(0));
  BOOST_CHECK_EQUAL("{{0", FORMAT("{{{{{}")(0));
  BOOST_CHECK_EQUAL("0}}", FORMAT("{}}}}}")(0));
}

BOOST_AUTO_TEST_CASE(empty_spec) {
  BOOST_CHECK_EQUAL("0 1 2", FORMAT("{} {} {}")(0, 1, 2));
}

BOOST_AUTO_TEST_CASE(numeric_spec) {
  BOOST_CHECK_EQUAL("0", FORMAT("{0}")(0));
  BOOST_CHECK_EQUAL("0 1 2", FORMAT("{0} {1} {2}")(0, 1, 2));
  BOOST_CHECK_EQUAL("2 1 0", FORMAT("{2} {1} {0}")(0, 1, 2));
  BOOST_CHECK_EQUAL("10", FORMAT("{10}")(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10));
}

BOOST_AUTO_TEST_CASE(various) {
  BOOST_CHECK_EQUAL("asdf 1 1", FORMAT("{} {} {}")("asdf", 1.0, true));
}
