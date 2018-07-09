#include <boost/smart_ptr/detail/yield_k.hpp>
#include <atomic>




class spinLock
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
public:
	class scoped_lock
	{
	private:
		spinLock & sp_;
		scoped_lock(scoped_lock const &);
		scoped_lock& operator=(scoped_lock const&);
	public:
		explicit  scoped_lock(spinLock& sp): sp_(sp)
		{
			sp.lock();
		}
		~scoped_lock()
		{
			sp_.unlock();
		}
	};
};