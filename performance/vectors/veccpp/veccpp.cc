#include <benchmark/benchmark.h>

const size_t NUM_ITEMS_SMALL = 10;
const size_t NUM_ITEMS_BIG = 1000000;

static void big(benchmark::State& state) {
  for (auto _ : state) {
      std::vector<size_t> v;
      for (size_t i = 0; i < NUM_ITEMS_BIG; i++) {
          v.push_back(i);
      }
  }
}
BENCHMARK(big);

static void small(benchmark::State& state) {
  for (auto _ : state) {
      std::vector<size_t> v;
      for (size_t i = 0; i < NUM_ITEMS_SMALL; i++) {
          v.push_back(i);
      }
  }
}
BENCHMARK(small);

BENCHMARK_MAIN();