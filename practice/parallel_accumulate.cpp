#include <thread>
#include <chrono>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <mutex>
using namespace std::chrono_literals;
void fun(int i)
{
	std::cout << "i ==" << i;
	std::this_thread::sleep_for(2s);
}

template<class It,class T>
struct accumulate_block
{
	void operator()(It first,It last,T& result)
	{
		result = std::accumulate(first, last, result);
	}
};
template<typename It,typename T>
T parallel_accumulate(It first,It last,T init)
{
	unsigned long const length = std::distance(first, last);
	if (!length)
		return init;
	unsigned long const min_per_thread = 25;
	unsigned long const max_threads =
		(length + min_per_thread - 1) / min_per_thread;
	unsigned long const hardware_threads =
		std::thread::hardware_concurrency();
	unsigned long const num_threads =
		std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
	std::vector<T> results(num_threads);
	std::vector<std::thread> threads(num_threads - 1);
	unsigned long const block_size = length / num_threads;

	It block_start = first;
	for(unsigned long i=0;i < (num_threads-1);++i)
	{
		It block_end = block_start;
		std::advance(block_end, block_size);
		threads[i] = std::thread(
			accumulate_block<It, T>(),
			block_start, block_end, std::ref(results[i]));
		block_start = block_end;
	}
	accumulate_block<It, T>()(block_start, last, results[num_threads - 1]);

	std::for_each(threads.begin(), threads.end(),
		std::mem_fn(&std::thread::join));
	return std::accumulate(results.begin(), results.end(), init);
}







//int main(int argc, char* argv[])
//{
//	int x = std::thread::hardware_concurrency();
//	std::vector<std::thread> v;
//	v.reserve(10);
//	for(auto i = 0;i < 10;++i)
//	{
//		v.emplace_back(fun, i);
//	}
//	std::for_each(v.begin(), v.end(), std::mem_fn(&std::thread::join));
//	
//	return x;
//	/*std::mutex m1, m2;
//	if (&m1 == &m2) return x;
//	std::lock(m1, m2);
//	std::lock_guard<std::mutex> lock_a(m1, std::adopt_lock);
//	std::lock_guard<std::mutex> lock_b(m2, std::adopt_lock);
//	*/
//
//
//}
//
