#include <random>
#include <numeric>
#include <benchmark/benchmark.h>

int  sum_handwritten(int* first, int* last, int init) {
	for (; first != last; ++first) init += *first;
	return init;
}

float sum_handwritten(float* first, float* last, float& init) {
	for (; first != last; ++first) init += *first;
	return init;

}
//generalt guideline may be this 
//don't pass by reference cause it introduces data dependency inside of any loop and will not permit 
//loop unrolling and vectorization.
float z = 0;
void bm_accumulate_slow(benchmark::State& state)
{
	std::vector<float> v(12014, 0);
	std::mt19937 gen(std::random_device{}());
	const std::uniform_real_distribution<double> dist(1.0f, 2.4f);
	std::generate(v.begin(), v.end(), [&]() {return dist(gen); });
	for (auto _ : state)
	{
		float sum = 0.0;
		sum_handwritten(&v.front(), &v.back(), z);
		benchmark::DoNotOptimize(sum);
		benchmark::ClobberMemory();
	}
	benchmark::DoNotOptimize(state.iterations());
	state.SetLabel("slow_accumulation");
}

void bm_accumulate_fast(benchmark::State& state)
{
	std::vector<float> v(2014, 0);
	std::mt19937 gen(std::random_device{}());
	const std::uniform_real_distribution<double> dist(1.0f, 2.4f);
	std::generate(v.begin(), v.end(), [&]() {return dist(gen); });
	for (auto _ : state)
	{
		float sum = 0.0;
		sum = std::accumulate(v.begin(), v.end(), z);
		benchmark::DoNotOptimize(sum);
		benchmark::ClobberMemory();
	}
	benchmark::DoNotOptimize(state.iterations());
	state.SetLabel("fast_6x_time_accumulation");
}
BENCHMARK(bm_accumulate_fast);
BENCHMARK(bm_accumulate_slow);