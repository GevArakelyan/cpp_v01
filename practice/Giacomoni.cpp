#include <atomic>

//https://www.youtube.com/watch?v=74QjNwYAJ7M

template<typename T, size_t M> class SPSC_Giacomoni
{
	std::atomic<T*> b_[M] = {};

	alignas(64) size_t h_ { 0 }; //head
	alignas(64) size_t t_ { 0 }; //tail

public:
	bool try_enq(T* p)
	{
		if (b_[t_].load(std::memory_order_acquire))
			return false;
		b_[t_].store(p, std::memory_order_release);
		t_ = Next(t_);
		return true;
	}
	size_t Next(size_t t)
	{
		return (t_ + 1) % M;
	}

	T* try_deq()
	{
		T* p = b_[h_].load(std::memory_order_acquire);
		if (!p) return nullptr;
		b_[h_].store(nullptr, std::memory_order_release);
		h_ = Next(h_);
		return p;
	}
};
