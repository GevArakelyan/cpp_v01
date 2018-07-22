#pragma once

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

