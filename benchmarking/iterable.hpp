#pragma once

#include <random>

template<class It>
class iterable
{
	It m_first_, m_last_;
public:
	iterable() = default;
	iterable(It first, It last) :m_first_(first), m_last_(last)
	{}
	It begin() const { return m_first_; }
	It end() const { return m_last_; }
};

template<class It>
inline iterable<It> make_iterable(It a, It b)
{
	return iterable<It>(a, b);
}

std::uniform_int_distribution<> rand_engine;
std::mt19937_64 gen;
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