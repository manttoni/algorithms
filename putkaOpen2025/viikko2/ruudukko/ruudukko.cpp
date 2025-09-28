#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <limits>

struct boksi
{
	std::vector<int> luvut;
	int n;
	int m;
	std::stringstream& ss;
};

bool can_add(int n, int i, const boksi info)
{
	int size = info.m * info.n;
	for (auto dir : {-1, 1, -info.m, info.m})
	{
		int check_idx = i + dir;
		if (check_idx < 0 || check_idx >= size)
			continue;
		if (info.luvut[check_idx] == 0)
			continue;
		if (std::abs(info.luvut[check_idx] - n) == 1)
			return false;
	}
	return true;
}

// test all placements starting from smallest unused num
// return true if found solution, otherwise false
bool fill(boksi& info, int idx)
{
	auto& luvut = info.luvut;
	auto size = info.n * info.m;
	if (idx >= size)
		return false;
	for (int i = 1; i <= size; ++i)
	{
		// continue used
		if (std::find(luvut.begin(), luvut.end(), i) != luvut.end())
			continue;

		// continue if luvut[idx] is not allowed at idx
		if (can_add(i, idx, info) == false)
			continue;

		// add a valid number
		luvut[idx] = i;

		// check if all numbers are in
		if (std::find(luvut.begin(), luvut.end(), 0) == luvut.end())
		{
			info.ss << "YES" << std::endl;
			for (int j = 0; j < size; ++j)
			{
				info.ss << luvut[j];
				if (j % info.m == info.m - 1)
					info.ss << std::endl;
				else
					info.ss << " ";
			}
			return true;
		}

		// add a number to next index
		bool ret = fill(info, idx + 1);


		if (ret == true)
			return true;

		// backtrack
		luvut[idx] = 0;
	}
	return false;
}

int main(void)
{
	int n, m, t;
	std::cin >> t;
	std::stringstream main_ss;
	for (int i = 0; i < t; ++i)
	{
		std::cin >> n >> m;
		std::stringstream ss;
		boksi info = {std::vector<int>(n * m), n, m, ss};
		for (size_t s = 0; s < n * m; ++s)
			info.luvut[s] = 0;
		if (fill(info, 0) == false)
			info.ss << "NO" << std::endl;
		main_ss << info.ss.str();
	}
	std::cout << main_ss.str();
	return 0;
}

