#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <cassert>
#include <cstdint>
#include <bit>

std::vector<std::string> members;

bool has_something_new(const std::string& candidate, const std::string& member)
{
	for (const char c : candidate)
	{
		if (member.find(c) == std::string::npos) // Member hasnt visited country c but candidate has
			return true;
	}
	return false;
}

bool can_join(const std::string& candidate)
{
	for (const auto& member : members)
	{
		// Each member has to have something more than any other
		if (!has_something_new(member, candidate))
			return false;
		if (!has_something_new(candidate, member))
			return false;
	}
	return true;
}

void solve()
{
	std::vector<uint16_t> all_combos;
	for (uint16_t i = 1; i <= (1 << 12); ++i)
	{
		if (std::popcount(i) > 10 || std::popcount(i) < 6)
			continue;
		all_combos.push_back(i);
	}

	for (const auto combo : all_combos)
	{
		std::string candidate = "";
		for (size_t i = 0; i < 12; ++i)
		{
			if (combo >> i & 1)
				candidate += 'A' + i;
		}
		if (!can_join(candidate))
			continue;

		members.push_back(candidate);

		std::sort(members.begin(), members.end());
	}

	for (const auto member : members)
		std::cout << member << std::endl;

}

int main(void)
{
	solve();
}
