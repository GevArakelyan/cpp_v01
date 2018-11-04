#include <boost/algorithm/string.hpp>
#include <algorithm>
#include <iostream>



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



int main(int argc, char* argv[])
{
	const std::string s = "hello world";
	std::string result = reverse(s);
	std::cout << result;
	return 0;
}
