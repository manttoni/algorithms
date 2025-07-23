#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

std::vector<size_t> primes;
std::map<size_t, size_t> pre;

bool is_prime(const size_t num)
{
	if (num < 2)
		return false;
	for (size_t p : primes)
	{
		if (num % p == 0)
			return false;
	}
	return true;
}

void init_primes(const size_t from, const size_t to)
{
	for (size_t i = from; i <= to; ++i)
	{
		if (is_prime(i))
			primes.push_back(i);
	}
}



size_t calculate(const size_t d)
{
	auto it = pre.find(d);
	if (it != pre.end())
		return it->second;
	size_t variations = 0;
	if (d == 0)
		return 1;
	for (const size_t n : primes)
	{
		if (d < n)
			continue;
		variations += calculate(d - n);
	}
	return variations;
}

int main(int ac, char **av)
{
	if (ac != 3)
		return 1;
	size_t from = atoi(av[1]);
	size_t to = atoi(av[2]);
	init_primes(from, to);
	for (size_t i = from; i <= to; ++i)
	{
		size_t variations = calculate(i);
		pre.insert({i, variations});
		std::cout << i << " " << variations * 2 << std::endl;
	}
	return 0;
}
