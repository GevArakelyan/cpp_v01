#include <benchmark/benchmark.h>
#include <boost/smart_ptr/detail/spinlock_w32.hpp>
#include <mutex>
#include <atomic>

#define rep2(x) x x
#define rep4(x) rep2(x) rep2(x)
#define rep8(x) rep4(x) rep4(x)
#define rep16(x) rep8(x) rep8(x)
#define repeate(x) rep16(x) rep16(x)

boost::detail::spinlock sp;
int x = 0;
void bm_boost_spinlock(benchmark::State& state)
{

	for (auto _ : state)
	{
		repeate({ sp.lock(); x = 0; sp.unlock(); });
		benchmark::DoNotOptimize(x);
	}

	benchmark::DoNotOptimize(state.iterations());
}

std::mutex m;

void bm_mutex(benchmark::State& state)
{

	for (auto _ : state)
	{
		repeate({ std::lock_guard<std::mutex> lock(m); x = 0; });
		benchmark::DoNotOptimize(x);
	}

	benchmark::DoNotOptimize(state.iterations());
}
class Simple_spinlock
{
	std::atomic_flag flag;
public:
	Simple_spinlock()
		: flag{ ATOMIC_FLAG_INIT }
	{}
	void lock()
	{
		while (flag.test_and_set(std::memory_order_acquire));
	}
	void unlock()
	{
		flag.clear(std::memory_order_release);
	}
};
Simple_spinlock simple_spinlock;
void bm_Simple_Spin_lock(benchmark::State& state)
{

	for (auto _ : state)
	{
		repeate({ simple_spinlock.lock(); x = 0; simple_spinlock.unlock(); });
		benchmark::DoNotOptimize(x);
	}

	benchmark::DoNotOptimize(state.iterations());
}
class spin
{
public:
	std::atomic_flag v;
public:
	bool try_lock()
	{
		return !v.test_and_set(std::memory_order_acquire);
	}
	void lock()
	{
		for (unsigned k = 0; !try_lock(); ++k)
			boost::detail::yield(k);
	}
	void unlock()
	{
		v.clear(std::memory_order_release);
	}
};
spin spinlk;
void bm_fast_Spin_lock(benchmark::State& state)
{

	for (auto _ : state)
	{
		repeate({ spinlk.lock(); x = 0; spinlk.unlock(); });
		benchmark::DoNotOptimize(x);
	}

	benchmark::DoNotOptimize(state.iterations());
}
//BENCHMARK(bm_boost_spinlock)->ThreadRange(1, 8)->UseRealTime();
//BENCHMARK(bm_fast_Spin_lock)->ThreadRange(1, 8)->UseRealTime();
//BENCHMARK(bm_Simple_Spin_lock)->ThreadRange(1, 8)->UseRealTime();
//BENCHMARK(bm_mutex)->ThreadRange(1, 8)->UseRealTime();


//BENCHMARK_MAIN();