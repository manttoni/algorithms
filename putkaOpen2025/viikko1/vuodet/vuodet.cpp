#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include <sstream>

bool has_digits(std::string year, const std::string leetyear)
{
	int i = 0;
	for (const char c : leetyear)
		if (year[i] == c)
			i++;

	return i == 4;
}

int main(void)
{
	int a;
	int b;

	std::string input;
	std::getline(std::cin, input);
	std::istringstream inputs(input);
	inputs >> a >> b;

	std::vector<int> leetvuodet;

	for (int i = a; i <= b; ++i)
	{
		if (has_digits(std::to_string(i), std::to_string(i * 1337)))
			leetvuodet.push_back(i);
	}

	std::cout << leetvuodet.size() << std::endl;
	for (int v : leetvuodet)
	{
		std::cout << v << std::endl;
	}
}

