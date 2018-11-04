#include "coin.h"
#include <gsl/gsl>
#include <iostream>


namespace money
{
	coin::coin(uint64_t whole, uint64_t fraction) : m_whole(whole), m_fraction(fraction)
	{
	}

	bool operator==(const coin& lhs, const coin& rhs)
	{
		return lhs.m_whole == rhs.m_whole
			&& lhs.m_fraction == rhs.m_fraction;
	}

	bool operator!=(const coin& lhs, const coin& rhs)
	{
		return !(lhs == rhs);
	}

	bool operator<(const coin& lhs, const coin& rhs)
	{
		if (lhs.m_whole < rhs.m_whole)
			return true;
		if (rhs.m_whole < lhs.m_whole)
			return false;
		return lhs.m_fraction < rhs.m_fraction;
	}

	bool operator<=(const coin& lhs, const coin& rhs)
	{
		return !(rhs < lhs);
	}

	bool operator>(const coin& lhs, const coin& rhs)
	{
		return rhs < lhs;
	}

	bool operator>=(const coin& lhs, const coin& rhs)
	{
		return !(lhs < rhs);
	}

	coin& coin::operator+=(const coin& coin_b)
	{
		Expects(std::numeric_limits<uint64_t>::max() - m_whole > coin_b.m_whole);
		m_fraction += coin_b.m_fraction;
		m_whole += coin_b.m_whole;
		if (m_fraction >= 100)
		{
			m_whole += 1;
			m_fraction -= 100;
		}
		return *this;
	}

	coin& coin::operator-=(const coin& coin_b)
	{
		Expects(*this >= coin_b);
		m_whole -= coin_b.m_whole;
		m_fraction -= coin_b.m_fraction;
		if (m_fraction < 0)
		{
			m_whole -= 1;
			m_fraction += 100;
		}
		return *this;
	}

	coin& coin::operator*=(uint64_t number)
	{
		Expects(std::numeric_limits<uint64_t>::max() / m_whole > number);
		auto whole = m_whole * number;
		auto fraction = m_fraction * number;
		if (fraction >= 100)
		{
			const auto q = fraction / 100;
			whole += q;
			fraction -= q * 100;
		}
		Ensures(coin(whole, fraction) >= *this);
		*this = coin(whole, fraction);
		return *this;
	}

	coin& coin::operator/=(uint64_t number)
	{
		const uint64_t money = m_whole * 100 + m_fraction;
		const auto dev = money / number;
		m_whole = dev / 100;
		m_fraction = dev - m_whole * 100;
		return *this;
	}

	coin operator+(const coin& coin_a, const coin& coin_b)
	{
		coin c(coin_a);
		return c += coin_b;
	}

	coin operator-(const coin& coin_a, const coin& coin_b)
	{
		coin c(coin_a);
		return c -= coin_b;
	}
} //namespace money
