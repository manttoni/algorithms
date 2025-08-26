#include <iostream>
#include <stdexcept>
#include <cassert>
#include <algorithm>
#include <vector>

#define NAPPIEN_MAARA 10

int valaistus_nyt = 0;
std::vector<int> nahdyt_valaistukset = { 0 };

bool valaistus_nahty(int valaistus)
{
	auto it = std::find(nahdyt_valaistukset.begin(), nahdyt_valaistukset.end(), valaistus);
	return it != nahdyt_valaistukset.end();
}

void paina_nappia(int n)
{
	if (n < 0 || n > NAPPIEN_MAARA - 1)
		throw std::runtime_error("Nappeja on " + std::to_string(NAPPIEN_MAARA));

	if (valaistus_nahty(valaistus_nyt ^ (1 << n)))
		return;

	valaistus_nyt ^= (1 << n);
	nahdyt_valaistukset.push_back(valaistus_nyt);
	std::cout << n + 1 << std::endl;
}

void seuraava(int n)
{
	if (n >= NAPPIEN_MAARA)
		return;
	paina_nappia(n);
	seuraava(n + 1);
	paina_nappia(n);
	seuraava(n + 1);
}

int main(void)
{
	seuraava(0);


	return 0;
}
