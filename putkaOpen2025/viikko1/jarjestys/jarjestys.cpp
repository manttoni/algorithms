#include <iostream>
#include <algorithm>
#include <vector>
#include <sstream>

class Pari
{
	private:
		int a, b;
	public:
		int get_a() const { return a; }
		int get_b() const { return b; }
		Pari(const std::string& str)
		{
			std::istringstream asd(str);
			asd>>a>>b;
		}
		bool operator<(const Pari& other) const
		{
			return b <= other.a;
		}
		friend std::ostream& operator<<(std::ostream& os, const Pari& pari)
		{
			os << pari.a << " " << pari.b;
			return os;
		}
};

std::stringstream ss;

bool in_order(const std::vector<Pari>& sorted)
{
	for (size_t i = 0; i < sorted.size() - 1; ++i)
	{
		if (sorted[i] < sorted[i + 1])
			continue;
		return false;
	}
	return true;
}

bool muodostaa_jonon(std::vector<Pari> sorted, std::vector<Pari> unsorted, int u, int s)
{
	if (u != -1 && s != -1)
	{
		sorted.insert(sorted.begin() + s, unsorted[u]);
		unsorted.erase(unsorted.begin() + u);
	}

	if (!in_order(sorted))
		return false;

	if (unsorted.empty())
	{
		if (in_order(sorted))
		{
			ss << "YES" << std::endl;
			for (const Pari& p : sorted)
				ss << p << std::endl;
			return true;
		}
	}

	for (size_t u = 0; u < unsorted.size(); ++u)
	{
		for (size_t s = 0; s < sorted.size(); ++s)
		{

/*
			if (s < sorted.size() - 1 && sorted[s + 1] < sorted[s])
			{
				if (sorted[s + 1].get_a() >= sorted[s].get_a() && sorted[s + 1].get_b() <= sorted[s].get_b())
				{
					std::swap(sorted[s], sorted[s + 1]);
					if (muodostaa_jonon(sorted, unsorted, -1, -1))
						return true;
					std::swap(sorted[s], sorted[s + 1]);
				}
			}*/
			int sorted_a = sorted[s].get_a();
			int sorted_b = sorted[s].get_b();
			int unsorted_a = unsorted[u].get_a();
			int unsorted_b = unsorted[u].get_b();

			int first_a = sorted.front().get_a();
			int last_b = sorted.back().get_b();
			if (unsorted_a == last_b)
				if (muodostaa_jonon(sorted, unsorted, u, sorted.size()))
					return true;
			if (unsorted_b == first_a)
				if (muodostaa_jonon(sorted, unsorted, u, 0))
					return true;

			bool left_better = abs(sorted_a - unsorted_b) <= abs(sorted_b - unsorted_a);

			if (left_better == true)
				if (unsorted[u] < sorted[s] && (s == 0 || sorted[s - 1] < unsorted[u]))
					if (muodostaa_jonon(sorted, unsorted, u, s))
						return true;
			if (sorted[s] < unsorted[u] && (s == sorted.size() - 1 || unsorted[u] < sorted[s + 1]))
				if (muodostaa_jonon(sorted, unsorted, u, s + 1))
					return true;
			if (left_better == false)
				if (unsorted[u] < sorted[s] && (s == 0 || sorted[s - 1] < unsorted[u]))
					if (muodostaa_jonon(sorted, unsorted, u, s))
						return true;
		}
	}
	return false;
}

int main(void)
{
	int t;
	std::cin >> t;
	for (int ti = 0; ti < t; ++ti)
	{
		int n;
		std::cin >> n;
		std::string asd;
		std::getline(std::cin, asd);
		std::vector<Pari> parit;
		parit.reserve(n);
		for (int ni = 0; ni < n; ++ni)
		{
			std::string line;
			std::getline(std::cin, line);
			parit.push_back(Pari(line));
		}
		if (parit.empty())
			continue;
		std::sort(parit.begin(), parit.end());
		if (!muodostaa_jonon({}, parit, 0, 0))
			ss << "NO" << std::endl;
	}
	std::cout << ss.str();
}

