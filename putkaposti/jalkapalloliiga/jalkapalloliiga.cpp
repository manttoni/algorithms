#include <random>
#include <vector>
#include <iostream>
#include <algorithm>

void print_joukkueet(std::vector<std::vector<int>> j)
{
	for (std::vector<int> v : j)
	{
		for (int i : v)
		{
			std::cout << i << " ";
		}
		std::cout << std::endl;
	}
}

void solve(void)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::vector<int> v;
	for (int i = 1; i <= 12; ++i)
		v.push_back(i);

	std::vector<std::vector<int>> j;
	int ite = 0;
	do {
		std::shuffle(v.begin(), v.end(), gen);
		for (int asd = 0; asd < 10; asd += 1)
		{
			std::vector<int> c = {v[0 + asd],v[1 + asd],v[2 + asd]};
			bool ei_voi_lisata = false;
			for (auto y : j)
			{
				int s = 0;
				for (int i : y)
				{
					if (std::find(c.begin(), c.end(), i) != c.end())
						s++;
				}
				if (s > 1)
				{
					ei_voi_lisata = true;
					break;
				}
			}
			if (ei_voi_lisata)
				continue;
			j.push_back(c);
		}
		ite++;
	} while (ite < 1000);
	if (j.size() >= 20)
	{
		print_joukkueet(j);
		exit(0);
	}

}

int main(void)
{
	while (true)
		solve();
}
