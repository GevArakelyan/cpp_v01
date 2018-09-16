#include <benchmark/benchmark.h>
#include <vector>
#include <random>
static std::uniform_int_distribution<> rand_engine;
static std::mt19937_64 gen;
template<class T>
struct RNG
{
	RNG(T min, T max, size_t count_) : first(min)
		, last(max)
		, count(count_)
	{}


	struct range_iterator
	{
		size_t count;
		range_iterator(size_t count_,
			T first, T last)
			: count(count_)
			, dist(first, last)
		{
			value = dist(gen);
		}
		std::uniform_int_distribution<> dist;
		T value;
		T operator*()
		{
			return value;
		}
		range_iterator& operator++()
		{
			--count;
			value = dist(gen);
			return *this;
		}
		range_iterator operator++(int)
		{
			range_iterator res = *this;
			--count;
			++*this;
			return res;
		}
		bool operator ==(range_iterator& other)
		{
			return count == other.count;
		}
		bool operator !=(range_iterator& other)
		{
			return !operator==(other);
		}
	};
	T first;
	T last;
	size_t count;
	range_iterator begin() const { return range_iterator{ count,first,last }; }
	range_iterator end() const { return range_iterator{ 0,first,last }; }
};


static void cachBench(benchmark::State& s)
{
	const int bytes = 1 << s.range(0);
	const int count = (bytes / (sizeof(int))) / 2;
	std::vector<int> v(count);
	for (auto i : RNG<int>(std::numeric_limits<int>::min(),
		std::numeric_limits<int>::max(), count))
	{
		v.push_back(i);
	}
	std::vector<int> indices;
	for (auto i : RNG<int>(0, count, count))
	{
		indices.push_back(i);
	}
	for (auto _ : s)
	{
		long sum = 0;
		for (int i : indices)
			sum += v[i];
		benchmark::DoNotOptimize(sum);
	}
	s.SetBytesProcessed(s.iterations()* bytes);
	s.SetLabel(std::to_string(bytes / 1024) + "__kb");

}

//
BENCHMARK(cachBench)->DenseRange(13, 27)->ReportAggregatesOnly(true);
//
//BENCHMARK_MAIN();