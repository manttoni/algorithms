#include <iostream>
#include <vector>
#include <numeric>
#include <sstream>
#include <algorithm>

int summa(std::vector<int> v, size_t alku = 0)
{
	int summa = 0;
	for (size_t i = alku; i < v.size(); ++i)
		summa += v[i];
	return summa;
}

std::stringstream ss;

bool solve(std::vector<int> num, size_t i, int k, std::vector<std::vector<int>> kasat)
{
	if (i == num.size()) // num is "empty"
	{
		if (summa(kasat[0]) == summa(kasat[1]) && summa(kasat[1]) == summa(kasat[2]))
		{
			ss << "YES" << std::endl;
			for (int j = 0; j < 3; ++j)
			{
				std::reverse(kasat[j].begin(), kasat[j].end());
				ss << kasat[j].size() << std::endl;
				for (int luku : kasat[j])
					ss << luku << " ";
				ss << std::endl;
			}
			return true;
		}
		return false;
	}

	kasat[k].push_back(num[i]);
	if (summa(kasat[k]) > summa(num) / 3)
		return false;
	int suurinsumma = summa(std::max(kasat[0], std::max(kasat[1], kasat[2])));
	int pieninsumma = summa(std::min(kasat[0], std::min(kasat[1], kasat[2])));
	if (suurinsumma > pieninsumma + summa(num, i + 1))
		return false;
	for (int j = 0; j < 3; ++j)
	{
		if (solve(num, i + 1, j, kasat) == true)
			return true;
	}
	return false;
}

int main(void)
{
	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
	{
		int n;
		std::cin >> n;
		std::vector<int> num(n);
		std::iota(num.begin(), num.end(),1);
		if (summa(num) % 3 != 0)
		{
			ss << "NO" << std::endl;
			continue;
		}
		std::reverse(num.begin(), num.end());

		std::vector<std::vector<int>> kasat(3);
		for (int j = 0; j < 3; ++j)
		{
			if (solve(num, 0, j, kasat))
				break;
			ss << "NO" << std::endl;
			break;
		}
	}
	std::cout << ss.str();
}
