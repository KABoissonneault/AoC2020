#include "day3.h"

#include <iostream>

namespace kab::day3
{
	int count_encountered_trees(std::istream& i, int horizontal_stride, int vertical_stride = 1)
	{
		int x = 0, y = 0;
		int encountered = 0;

		std::string line;
		while (std::getline(i, line))
		{
			if (y++ % vertical_stride != 0)
				continue;

			if (line[x] == '#')
				++encountered;

			x += horizontal_stride;
			x %= line.size();
		}
		
		return encountered;
	}

	std::string solve_1(std::istream& i)
	{
		return std::to_string(count_encountered_trees(i, 3));
	}

	std::string solve_2(std::istream& i)
	{
		uint64_t result1 = count_encountered_trees(i, 1);
		i.clear();
		i.seekg(0, std::ios_base::beg);
		
		uint64_t result2 = count_encountered_trees(i, 3);
		i.clear();
		i.seekg(0, std::ios_base::beg);
		
		uint64_t result3 = count_encountered_trees(i, 5);
		i.clear();
		i.seekg(0, std::ios_base::beg);
		
		uint64_t result4 = count_encountered_trees(i, 7);
		i.clear();
		i.seekg(0, std::ios_base::beg);
		
		uint64_t result5 = count_encountered_trees(i, 1, 2);
		
		return std::to_string(result1 * result2 * result3 * result4 * result5);
	}
}
