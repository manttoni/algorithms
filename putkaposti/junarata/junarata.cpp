#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <cassert>
#include <numeric>
#include <climits>
#include <iomanip>
#include <cmath>
#include <unordered_map>
#include <algorithm>

bool PRINT = true;

const std::vector<std::string> solutions = {"5 2 3", "10 3 1", "50 23 31", "100 49 167", "500 254 4782", "1000 501 17238"};

class Train
{
	private:
		int length;						// amount of numbers
		int least_loops;				// least amount of loops needed
		int swaps;						// different ways to swap to get least_loops
		std::vector<int> numbers;
		std::vector<int> indices;		// indices[n] = index of n. indices[0] = -1
		std::map<int, int> number_ids;	// key=number, value=index
		std::unordered_map<int, std::vector<int>> checked;

		int loop() const
		{
			return loop(0, length - 1);
		}

		int loop(int start_id, int end_id) const
		{
			int loops = 1;
			int prev_value = 0;
			for (int i = start_id; i <= end_id; ++i)
			{
				int value = number_ids[i + 1];
				if (prev_value > value)
					loops++;
				prev_value = value;
			}
			return loops;
		}

		void read_numbers(const std::string &filename)
		{
			std::ifstream file(filename);
			if (!file.is_open())
				throw std::runtime_error("Reading " + filename + " failed");
			file >> length;
			int number, index = 0;
			while (file >> number)
				numbers.push_back(number);
			for (int n : numbers)
				number_ids[n] = index++;

		}

		void swap(const int a, const int b)
		{
			const int a_id = number_ids[a];
			const int b_id = number_ids[b];

			// swap indices
			int tmp = number_ids[a];
			number_ids[a] = number_ids[b];
			number_ids[b] = tmp;

			// swap numbers
			tmp = a;
			numbers[a_id] = numbers[b_id];
			numbers[b_id] = a;
		}

		bool swap_checked(const int a, const int b)
		{
			int key = std::min(a, b);
			int value = std::max(a, b);
			if (checked.find(key) == checked.end())
				return false;
			auto list = checked[key];
			return std::find(list.begin(), list.end(), value) != list.end();
		}

		std::string check_swap(const int a, const int b)
		{
			assert(a > 0 && b > 0 && a <= length && b <= length);
			if (a == b)
				return "Skip";

			if (swap_checked(a, b))
				return "Skip";

			swap(a, b); // be careful to not use numbers vector after swapping
			const int loops = loop();
			swap(a, b);

			std::string result;
			if (loops == least_loops)
			{
				swaps++;
				result = "Equal";
			}
			else if (loops < least_loops)
			{
				least_loops = loops;
				swaps = 1;
				result = "Better";
			}
			else
				result = "Worse";

			if (PRINT)
			{
				std::cout	<< std::setw(12) << std::left << (std::to_string(a) + " <=> " + std::to_string(b))
							<< std::setw(16) << std::left << ("i:" + std::to_string(number_ids[a]) + " <i> i:" + std::to_string(number_ids[b]))
							<< std::setw(12) << std::left << ("Loops: " + std::to_string(loops));
				if (result == "Worse") std::cout << "\033[31m";
				if (result == "Better") std::cout << "\033[32m";
				std::cout << std::setw(7) << std::left << result << "\033[0m" << std::endl;
			}
			int key = std::min(a, b);
			int value = std::max(a, b);
			checked[key].push_back(value);
			return result;
		}

		// returns distance between two numbers
		int distance(const int a, const int b)
		{
			assert(a > 0 && b > 0 && a <= length && b <= length);
			return (length + number_ids[b] - number_ids[a]) % length;
		}

	public:
		unsigned int get_length() const { return length; }
		unsigned int get_least_loops() const { return least_loops; }
		unsigned int get_swaps() const { return swaps; }

		Train(const std::string &filename) : swaps(0)
		{
			read_numbers(filename);
			least_loops = loop();
		}

		void print_numbers() const
		{
			std::cout << "\033[34m";
			for (const int n : numbers)
				std::cout << n << "\t";
			std::cout << "\033[0m" << std::endl;
			for (int i = 0; i < length; ++i)
				std::cout << i << "\t";
			std::cout << std::endl;
		}

		friend std::ostream &operator<<(std::ostream &os, const Train &t)
		{
			os << t.length << " " << t.least_loops << " " << t.swaps;
			return os;
		}

		bool is_left_of(const int a, const int b)
		{
			if (number_ids[a] < number_ids[b])
			{
				std::cout << a << " is left of " << b << std::endl;
				return true;
			}
			return false;
		}

		bool is_right_of(const int a, const int b)
		{
			if (number_ids[a] > number_ids[b])
			{
				std::cout << a << " is right of " << b << std::endl;
				return true;
			}
			return false;
		}


		bool verify_order(int left, int mid, int right)
		{
			return number_ids[left] < number_ids[mid] && number_ids[mid] < number_ids[right];
		}

		std::pair<int, int> get_range(const int n)
		{
			std::pair<int, int> range;

			// BEGIN
			if (n == 1)
				range.first = 0;
			else
			{
				int smaller = n - 1;
				int small_index = number_ids[smaller];
				if (small_index > number_ids[n]) // smaller is on the right side so can swap with it
					range.first = small_index;
				else
					range.first = small_index + 1;
			}

			// END
			if (n == length)
				range.second = length - 1;
			else
			{
				int bigger = n + 1;
				int big_index = number_ids[bigger];
				if (big_index < number_ids[n]) // bigger is on the left side so can swap with it
					range.second = big_index;
				else
					range.second = big_index - 1;
			}

			if (range.first == length - 1) // would have to begin after vector
				range.first = 0; // allow
			if (range.second == 0)
				range.second = length - 1;

			return range;
		}

		void solve()
		{
			for (int i = 0; i < length - 1; ++i)
			{
				for (int j = i + 1; j < length; ++j)
				{
					std::cout << "Swapping (maybe) " << numbers[i] << " and " << numbers[j] << std::endl;
					std::cout << "Loops before: " << least_loops << std::endl;

					int a_before = loop((i == 0) ? 0 : i - 1, i + 1);
					int b_before = loop(j - 1, (j + 1 == length) ? j : j + 1);

					std::cout << "a: " << ((i == 0) ? numbers[0] : numbers[i - 1]) << " -> " << numbers[i + 1] << std::endl;
					std::cout << "b: " << numbers[j - 1] << " -> " << ((j == length - 1) ? numbers[j] : numbers[j + 1]) << std::endl;
					std::cout << "a_before: " << a_before << std::endl;
					std::cout << "b_before: " << b_before << std::endl;

					swap(numbers[i], numbers[j]);

					print_numbers();

					std::cout << "Swapped" << std::endl;
					std::cout << "Loops after: " << loop() << std::endl;

					int a_after = 1 + (numbers[i] > numbers[i + 1]);
					if (i > 0)
						a_after += numbers[i - 1] > numbers[i];

					int b_after = 1 + (numbers[j - 1] > numbers[j]);
					if (j + 1 < length)
						b_after += numbers[j] > numbers[j + 1];

					std::cout << "a_after: " << a_after << std::endl;
					std::cout << "b_after: " << b_after << std::endl;

					swap(numbers[i], numbers[j]);

					if ((a_before + b_before) - (a_after + b_after) > 0)
						check_swap(numbers[i], numbers[j]);
				}
			}
		}
};

int main(void)
{
	for (size_t i = 0; i <= 0; ++i)
	{
		Train train("junar/junar" + std::to_string(i) + ".in");
		train.print_numbers();
		train.solve();
		std::cout << train << std::endl;

		if (i < solutions.size())
		{
			std::ostringstream oss;
			oss << train;
			std::string solution = oss.str();
			assert(solution == solutions[i]);
		}
	}

	return 0;
}
