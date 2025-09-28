#include <iostream>
#include <string>
#include <vector>

int main(void)
{
	size_t n, m, k;
	std::cin >> n >> m >> k;

	std::string leimasin;
	std::cin >> leimasin;

	std::string mj = std::string(n, '.');
	std::vector<size_t> kohdat;
	kohdat.reserve(k);
	for (size_t i = 0; i < k; ++i)
	{
		size_t kohta;
		std::cin >> kohta;
		kohdat.push_back(kohta - 1);
	}

	// iterate backwards and never replace non-'.' chars
	for (size_t i = kohdat.size() - 1; i < kohdat.size(); --i)
	{
		bool printtas = false;
		for (size_t j = 0; j < m; ++j)
		{
			if (mj[kohdat[i] + j] == '.')
			{
				mj[kohdat[i] + j] = leimasin[j];
				printtas = true;
			}
			else if (printtas == true) // if cant print now but did before, there wont be a .
				break;
			else // jump to next .
			{
				j = mj.find('.', kohdat[i] + j) - kohdat[i] - 1;
			}
		}
	}

	std::cout << mj << std::endl;
}
