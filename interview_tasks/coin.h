#pragma once

#include <cstdint>





namespace money
{
	class coin
	{
	public:

		coin(uint64_t whole, uint64_t fraction);

		coin& operator+=(const coin& coin_b);
		coin& operator-=(const coin& coin_b);
		coin& operator*=(uint64_t number);
		coin& operator/=(uint64_t number);

		friend bool operator==(const coin& lhs, const coin& rhs);

		friend bool operator!=(const coin& lhs, const coin& rhs);

		friend bool operator<(const coin& lhs, const coin& rhs);

		friend bool operator<=(const coin& lhs, const coin& rhs);

		friend bool operator>(const coin& lhs, const coin& rhs);

		friend bool operator>=(const coin& lhs, const coin& rhs);

		friend coin operator+(const coin& coin_a, const coin& coin_b);

		friend coin operator-(const coin& coin_a, const coin& coin_b);

	private:

		uint64_t m_whole = 0;
		uint64_t m_fraction = 0;
	};

} // namespace money
