#include <iostream>
#include <string>
#include <sstream>
#include <vector>

int main(void)
{
	int maara;
	std::cin >> maara;
	std::vector<double> tulokset;
	for (int i = 0; i < maara; ++i)
	{
		int n;
		std::cin >> n;
		double eivoittoa = 1;
		for (int j = 0; j < n; ++j)
		{
			eivoittoa *= (n - 1.0) / n;
		}
		tulokset.push_back(1-eivoittoa);
	}

	for (double t : tulokset)
		std::cout << t << std::endl;

}
