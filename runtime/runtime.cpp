#include <benchmark/benchmark.h>

#include <sstream>

#include <boost/format.hpp>

#include <format.hpp>

static void BM_Format(benchmark::State& state) {
  std::stringstream ss;
  while (state.KeepRunning()) {
    ss << FORMAT(
        "{0} asdfasdfasdfasdfasdfasdfasdfasd {0} "
        "asdfsadfasdfasdfasdfasdfasdfasdf"
        "{0} asdfasdfasdfasdfasdfasdfasdfasd {0} "
        "asdfsadfasdfasdfasdfasdfasdfasdf"
        "{0} asdfasdfasdfasdfasdfasdfasdfasd {0} "
        "asdfsadfasdfasdfasdfasdfasdfasdf"
        "{0} asdfasdfasdfasdfasdfasdfasdfasd {0} "
        "asdfsadfasdfasdfasdfasdfasdfasdf")(1);
  }
}
BENCHMARK(BM_Format);

static void BM_Boost(benchmark::State& state) {
  std::stringstream ss;
  while (state.KeepRunning()) {
    ss << boost::format(
              "%1% asdfasdfasdfasdfasdfasdfasdfasd %1% "
              "asdfsadfasdfasdfasdfasdfasdfasdf"
              "%1% asdfasdfasdfasdfasdfasdfasdfasd %1% "
              "asdfsadfasdfasdfasdfasdfasdfasdf"
              "%1% asdfasdfasdfasdfasdfasdfasdfasd %1% "
              "asdfsadfasdfasdfasdfasdfasdfasdf"
              "%1% asdfasdfasdfasdfasdfasdfasdfasd %1% "
              "asdfsadfasdfasdfasdfasdfasdfasdf") %
              1;
  }
}
BENCHMARK(BM_Boost);

BENCHMARK_MAIN();
