#include <boost/lockfree/spsc_queue.hpp>
#include <mutex>
#include <gsl/gsl>
#include <boost/thread/barrier.hpp>

struct my_barrier
{
	unsigned int m_generation;
	unsigned int m_count;
	std::mutex m_mutex;
	std::condition_variable m_cond;
	unsigned int const m_size;

	my_barrier(unsigned _count) : m_generation(0)
	                              , m_count(_count)
	                              , m_size(_count)
	{
		if (0 == _count)
		{
			throw std::invalid_argument("barrier thread count");
		}
	}

	my_barrier(my_barrier const&) = delete;
	my_barrier& operator=(my_barrier const&) = delete;

	bool wait()
	{
		std::unique_lock<std::mutex> key(m_mutex);
		const auto gen = m_generation;
		if(--m_count == 0)
		{
			m_generation++;
			m_count = m_size;
			Ensures(m_count != 0);
			key.unlock();
			m_cond.notify_all();
			return true;
		}
		while (gen == m_generation)
			m_cond.wait(key);
		return false;
	}
};
