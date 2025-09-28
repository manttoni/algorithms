#include <vector>
#include <map>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <algorithm>

class Board;

const std::vector<int> sizes = { 2, 4, 6, 10, 16, 26, 42, 68, 110, 178, 288, 466, 754, 1220, 1974, 3194, 5168, 8362 };
std::map<int, long long> results;

class Board
{
	private:
		int size;
		std::vector<int> knight_ids;

		int mandis(const int ida, const int idb) const
		{
			int x_a = ida % size;
			int x_b = idb % size;
			int y_a = ida / size;
			int y_b = idb / size;
			return abs(x_a - x_b) + abs(y_a - y_b);
		}

		bool is_threat(const int a, const int b) const
		{
			int ida = knight_ids[a];
			int idb = knight_ids[b];

			if (mandis(ida, idb) != 3)
				return false;

			if (abs(ida / size - idb / size) > 2)
				return false;

			if (ida - 2 * size - 1 == idb ||
				ida - 2 * size + 1 == idb ||
				ida + 2 * size - 1 == idb ||
				ida + 2 * size + 1 == idb ||
				ida - 2 - size == idb ||
				ida - 2 + size == idb ||
				ida + 2 - size == idb ||
				ida + 2 + size == idb)
				return true;
			return false;
		}

		bool knight_at(const int i) const
		{
			auto it = std::find(knight_ids.begin(), knight_ids.end(), i);
			return it != knight_ids.end();
		}

		bool is_valid() const
		{
			if (knight_ids.size() < 2 || knight_ids.size() > 3)
				return true;

			for (size_t i = 0; i < knight_ids.size(); ++i)
			{
				for (size_t j = i + 1; j < knight_ids.size(); ++j)
				{
					if (is_threat(i, j))
						return false;
				}
			}
			return true;
		}

		// check if all knights are in the same sub board
		bool knights_in_same_sub(int i, int j, int k)
		{
			int ir = i / size;
			int ic = i % size;
			int jr = j / size;
			int jc = j % size;
			int kr = k / size;
			int kc = k % size;
			int sub_size = size / 2;
			if (ir / sub_size != jr / sub_size ||
				ir / sub_size != kr / sub_size)
				return false;
			if (ic / sub_size != jc / sub_size ||
				ic / sub_size != kc / sub_size)
				return false;
			return true;
		}

	public:
		Board() : size(0) {}
		Board(int size) : size(size) {}
		~Board() {}

		int get_size() const { return size; }

		void remove_last()
		{
			knight_ids.pop_back();
		}

		// try to add knight, return success
		bool add_knight(int y, int x) { return add_knight(y * size + x); }
		bool add_knight(int index)
		{
			knight_ids.push_back(index);
			if (knight_ids.size() > 1 && index / size - knight_ids[knight_ids.size() - 2] / size > 2)
				return true;

			if (!is_valid())
			{
				remove_last();
				return false;
			}
			//print();
			return true;
		}
		void print() const
		{
			// top
			for (int i = 0; i < size * 2 + 3; ++i)
				std::cout << "=";
			std::cout << std::endl;

			// middle
			for (int i = 0; i < size * size; ++i)
			{
				if (i % size == 0)
					std::cout << "| ";
				std::cout << knight_at(i) << " ";
				if ((i + 1) % size == 0)
					std::cout << "|" << std::endl;
			}

			// bottom
			for (int i = 0; i < size * 2 + 3; ++i)
				std::cout << "=";
			std::cout << std::endl;
		}

		long long get_result()
		{
			if (size == 2)
			{
				results[2] = 4;
				return 4;
			}
			auto last = results.end();
			last--;
			int pre_size = last->first;
			int X = size - pre_size;
			long long result = last->second + X * (pre_size * 2 - 1);
			std::cout << "Pre size: " << pre_size << " | X: " << X << " | result: " << result << std::endl;

			for (int i = 0; i < size * size - 2; ++i)
			{
				add_knight(i);
				for (int j = i + 1; j < size * size - 1; ++j)
				{
					if (add_knight(j))
					{
						for (int k = j + 1; k < size * size; ++k)
						{

							for (int l = 0; l < X + 1; ++l)
							{
								if (i / size >= l && i % size >= l &&
									j / size >= l && j % size >= l &&
									k / size >= l && k % size >= l &&
									i / size < l + pre_size && i % size < l + pre_size &&
									j / size < l + pre_size && j % size < l + pre_size &&
									k / size < l + pre_size && k % size < l + pre_size)
								{
									k += (pre_size + l) - (k % size);
									break;
								}
							}
							if (k < size * size && add_knight(k))
							{
								result++;
								remove_last();
							}
						}
						remove_last();
					}
				}
				remove_last();
			}
			results[size] = result;
			return result;
		}
};

int main(void)
{
	std::vector<int> correct = { 4, 276, 4752, 135040, 2555252, 49655552 };
	for (int i = 0; i < sizes.size(); ++i)
	{
		Board b(sizes[i]);
		long long result = b.get_result();
		std::cout << sizes[i] << " " << result << std::endl;
		if (i < correct.size())
			assert(result == correct[i]);
	}

	return 0;
}
