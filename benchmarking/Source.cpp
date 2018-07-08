#include <benchmark/benchmark.h>

void bm_empty(benchmark::State& state)
{
	while (state.KeepRunning())
	{
	}
	benchmark::DoNotOptimize(state.iterations());
}

BENCHMARK(bm_empty);


BENCHMARK_MAIN();