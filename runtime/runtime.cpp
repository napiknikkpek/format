#include <benchmark/benchmark.h>

#include <boost/format.hpp>

#include <format.hpp>

static void BM_Format(benchmark::State& state) {
  while (state.KeepRunning()) {
    FORMAT(
        "{0} asdfasdfasdfasdfasdfasdfasdfasd {0} "
        "asdfsadfasdfasdfasdfasdfasdfasdf"
        "{0} asdfasdfasdfasdfasdfasdfasdfasd {0} "
        "asdfsadfasdfasdfasdfasdfasdfasdf"
        "{0} asdfasdfasdfasdfasdfasdfasdfasd {0} "
        "asdfsadfasdfasdfasdfasdfasdfasdf"
        "{0} asdfasdfasdfasdfasdfasdfasdfasd {0} "
        "asdfsadfasdfasdfasdfasdfasdfasdf")
    (1);
  }
}
BENCHMARK(BM_Format);

static void BM_Boost(benchmark::State& state) {
  while (state.KeepRunning()) {
    boost::str(boost::format("%1% asdfasdfasdfasdfasdfasdfasdfasd %1% "
                             "asdfsadfasdfasdfasdfasdfasdfasdf"
                             "%1% asdfasdfasdfasdfasdfasdfasdfasd %1% "
                             "asdfsadfasdfasdfasdfasdfasdfasdf"
                             "%1% asdfasdfasdfasdfasdfasdfasdfasd %1% "
                             "asdfsadfasdfasdfasdfasdfasdfasdf"
                             "%1% asdfasdfasdfasdfasdfasdfasdfasd %1% "
                             "asdfsadfasdfasdfasdfasdfasdfasdf") %
               1);
  }
}
BENCHMARK(BM_Boost);

BENCHMARK_MAIN();
