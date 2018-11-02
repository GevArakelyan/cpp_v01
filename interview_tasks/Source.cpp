#include <iostream>
#include <string>

int factorial(int k)
{
	if (k == 0) return 1;
	int result = 1;
	for (int i = 2; i <= k; ++i)
		result *= i;
	return result;
}

int combinations(int k, int n)
{
	if (n < k)
		return 0;

	const int k_factorial = factorial(k);
	const int n_minus_k_factorial = factorial(n - k);

	return n_minus_k_factorial / k_factorial;
}

#include <boost/algorithm/string.hpp>
#include <algorithm>

std::string reverse(std::string const& sentence)
{
	std::string result;
	result.reserve(sentence.size());
	std::for_each(make_split_iterator(sentence, token_finder(boost::algorithm::is_space())),
	              boost::split_iterator<std::string::const_iterator>(), [&](auto& it)
	              {
		              std::reverse_copy(it.begin(), it.end(), std::back_inserter(result));
		              result.push_back(' ');
	              });

	return result;
}

class coin
{
public:

	friend bool operator<(const coin& lhs, const coin& rhs)
	{
		if (lhs.whole < rhs.whole)
			return true;
		if (rhs.whole < lhs.whole)
			return false;
		return lhs.fraction < rhs.fraction;
	}

	friend bool operator<=(const coin& lhs, const coin& rhs)
	{
		return !(rhs < lhs);
	}

	friend bool operator>(const coin& lhs, const coin& rhs)
	{
		return rhs < lhs;
	}

	friend bool operator>=(const coin& lhs, const coin& rhs)
	{
		return !(lhs < rhs);
	}

	coin& operator+=(const coin& coin_b);
	coin& operator-=(const coin& coin_b);
	coin& operator*=(uint64_t number);
	coin& operator/=(uint64_t number);

	friend coin operator+(const coin& coin_a, const coin& coin_b);
	friend coin operator+(const coin& coin_a, const coin& coin_b);

private:

	uint64_t whole = 0;
	uint64_t fraction = 0;
};

coin& coin::operator+=(const coin& coin_b)
{
	fraction += coin_b.fraction;
	whole += coin_b.whole;
	if (fraction >= 100)
	{
		whole += 1;
		fraction -= 100;
	}
	return *this;
}

coin& coin::operator-=(const coin& coin_b)
{
	whole -= coin_b.whole;
	fraction -= coin_b.fraction;
	if (fraction < 0)
	{
		whole -= 1;
		fraction += 100;
	}
	return *this;
}

coin& coin::operator*=(uint64_t number)
{
	whole *= number;
	fraction *= number;
	if (fraction >= 100)
	{
		const auto q = fraction / 100;
		whole += q;
		fraction -= q * 100;
	}
	return *this;
}

coin& coin::operator/=(uint64_t number)
{
	const uint64_t money = whole * 100 + fraction;
	const auto dev = money / number;
	whole = dev / 100;
	fraction = dev - whole * 100;
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


int main(int argc, char* argv[])
{
	const std::string s = "hello world";
	std::string result = reverse(s);
	std::cout << result;
	return 0;
}
