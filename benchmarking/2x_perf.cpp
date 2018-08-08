#include <benchmark/benchmark.h>
#include <vector>


void bm_loop_x(benchmark::State& state)
{
	std::vector<float> v1(16024, 1.021), v2(16024, 3.401);
	volatile int num = 20;
	volatile float d;
	for (auto _ : state)
	{
		for (auto coeff = 0; coeff < num; ++coeff)
		{
			for (unsigned long long sample = 0; sample < v1.size() - num; sample++)
				v2[coeff] += v1[sample] + v1[sample + coeff];
		}
		benchmark::DoNotOptimize(v1);
		benchmark::ClobberMemory();
	}
	benchmark::DoNotOptimize(state.iterations());
	state.SetLabel("loopx__time");
}

void bm_loop_more_than_2x(benchmark::State& state)
{
	std::vector<float> v1(16024, 1.021), v2(16024, 3.401);
	volatile int num = 20;
	volatile float d;
	for (auto _ : state)
	{
		for (auto coeff = 0; coeff < num; ++coeff)
		{
			auto d = 0.0f;
			for (unsigned long long sample = 0; sample < v1.size() - num; sample++)
				d += v1[sample] + v1[sample + coeff];
			v2[coeff] = d;
		}
		benchmark::DoNotOptimize(v1);
		benchmark::ClobberMemory();
	}
	benchmark::DoNotOptimize(state.iterations());
	state.SetLabel("loop2x__time");
}
BENCHMARK(bm_loop_more_than_2x);
BENCHMARK(bm_loop_x);


