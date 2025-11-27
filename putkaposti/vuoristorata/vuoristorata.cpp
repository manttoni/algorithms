#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <map>

bool can_see(const int seer, const std::vector<int>& kids)
{
	for (const int kid : kids)
	{
		if (kid < seer) // seer can see from behind kid
			continue;
		if (kid == seer) // Got to seer before finding anyone taller
			break;
		if (kid > seer) // This kid is taller than the kid behind(seer)
			return false;
	}
	return true;
}

int count_seers(const std::vector<int>& kids)
{
	const int tallest = kids.size() - 1;
	int seers = 0;
	for (const int kid : kids)
	{
		if (can_see(kid, kids))
			seers++;
		if (kid == tallest) // No kid can see if they are behind the tallest
			break;
	}
	return seers;
}

void solve(const int n)
{
	std::map<int, int> data;

	std::vector<int> kids;
	for (int i = 0; i < n; ++i)
		kids.push_back(i);

	int seers = 0;
	int permutations = 0;

	do {
		permutations++;
		int seers_this_permutation = count_seers(kids);

		if (permutations + 1 < permutations || seers + seers_this_permutation < seers)
			throw std::runtime_error("Overflow");

		seers += seers_this_permutation;

		if (data.find(seers_this_permutation) == data.end())
			data[seers_this_permutation] = 0;
		data[seers_this_permutation]++;
	} while (std::next_permutation(kids.begin(), kids.end()));

	int yt = std::gcd(seers, permutations);

	std::cout << n << " " << seers / yt << "/" << permutations / yt << std::endl;

	std::cout << "Data:" << std::endl;
	for (const auto& [key, value] : data)
		std::cout << "This many can see: " << key << " in this many cases: " << value << std::endl;

	if (n == 3)
	{
		assert(seers == 11);
		assert(permutations == 6);
	}
}

int main(void)
{
	for (int n = 3; n <= 30; ++n)
	{
		solve(n);
	}
}
