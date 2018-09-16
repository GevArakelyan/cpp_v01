#pragma once

#include <vector>
#include <iostream>


template <class Iterator>
decltype(auto) pack_argument(Iterator first)
{
	if constexpr (std::is_reference_v<decltype(*first)>)
	{
		return std::make_tuple(std::ref(*first));
	}
	else
	{
		return std::make_tuple(*first);
	}
}

template <int N, class Iterator>
decltype(auto) pack_arguments(Iterator first)
{
	if constexpr (N == 1)
	{
		return pack_argument(first);
	}
	else
	{
		return std::tuple_cat(pack_argument(first),
		                      pack_arguments<N - 1>(std::next(first)));
	}
}

template <int N, class Iterator, class Func, class = std::enable_if_t<(N > 0)>>
void for_each_adjacent(Iterator first, Iterator last, Func f)
{
	for (auto d = std::distance(first, last); d >= N; --d)
	{
		std::apply(f, pack_arguments<N>(first++));
	}
}


template<class Function,class Iterator,int...Is>
auto cpp14_custom_apply(Function f,Iterator first,std::integer_sequence<int,Is...>)
{
	return f(*std::next(first,Is)...);
}




template <int N, class Iterator, class Func, class = std::enable_if_t<(N > 0)>>
void cpp14_for_each_adjacent(Iterator first, Iterator last, Func f)
{
	for (auto d = std::distance(first, last); d >= N; --d)
	{
		cpp14_custom_apply(f, first++, std::make_integer_sequence<int, N>());
	}
}
//
//
//#include <vector>
//
//
//int main(int argc, char** argv)
//{
//	auto vec = std::vector<int>{{1, 2, 3, 4}};
//
//	cpp14_for_each_adjacent<3>(vec.begin(), vec.end(), [](auto ... vs)
//	{
//		(std::cout << ... << vs);
//	});
//	return 0;
//}
