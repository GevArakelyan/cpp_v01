#include <cstdint>
#include <atomic>









//https://www.youtube.com/watch?v=74QjNwYAJ7M
//1983

template<typename T, size_t M> class SPSCLamport
{
	T* b_[M];
	std::atomic<uint64_t> h_{ 0 };
	std::atomic<uint64_t> t_{ 0 };
public:
	bool try_enq(T* p)
	{
		auto t = t_.load(std::memory_order_acquire);
		if (h_.load(std::memory_order_acquire) + M == t)
			return false;
		b_[t%M] = p;
		t_.store(t + 1, std::memory_order_acquire);
		return true;
	}
	T* try_deq()
	{
		auto h = h_.load(std::memory_order_acquire);
		if (t_.load(std::memory_order_acquire) == h)
			return nullptr;
		T* p = b_[h%M];
		h_.store(h + 1, std::memory_order_release);
		return p;
	}



};
