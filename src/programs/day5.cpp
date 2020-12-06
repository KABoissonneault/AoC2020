#include "programs.h"

#include <iostream>
#include <bitset>
#include <string_view>
#include <vector>
#include <algorithm>

namespace kab::day5
{
	std::bitset<7> get_row(std::string_view line)
	{
		std::bitset<7> row;
		for (int i = 0; i < 7; ++i)
		{
			if (line[6 - i] == 'B')
			{
				row.set(i, true);
			}
			else if (line[6 - i] == 'F')
			{
				row.set(i, false);
			}
			else
			{
				throw std::runtime_error("Invalid input");
			}
		}
		return row;
	}

	std::bitset<3> get_seat(std::string_view line)
	{
		std::bitset<3> seat;
		for (int i = 0; i < 3; ++i)
		{
			if (line[9 - i] == 'R')
			{
				seat.set(i, true);
			}
			else if (line[9 - i] == 'L')
			{
				seat.set(i, false);
			}
			else
			{
				throw std::runtime_error("Invalid input");
			}
		}
		return seat;
	}

	unsigned long long get_id(std::string_view line)
	{
		std::bitset<7> const row = get_row(line);
		std::bitset<3> const seat = get_seat(line);
		return row.to_ullong() << 3 | seat.to_ullong();
	}

	std::string solve_1(std::istream& i)
	{
		std::vector<unsigned long long> ids;

		std::string line;
		while (std::getline(i, line))
		{
			if (line.size() != 10) throw std::runtime_error("Invalid input");
			ids.push_back(get_id(line));
		}

		unsigned long long const max_id = *std::ranges::max_element(ids);
		
		return std::to_string(max_id);
	}

	std::string solve_2(std::istream& i)
	{
		std::vector<unsigned long long> ids;

		std::string line;
		while (std::getline(i, line))
		{
			if (line.size() != 10) throw std::runtime_error("Invalid input");
			ids.push_back(get_id(line));
		}

		std::ranges::sort(ids);

		unsigned long long previous_seat = ids.front();
		for (int id = 1; id < ids.size(); ++id)
		{
			unsigned long long current_seat = ids[id];
			if (current_seat != previous_seat + 1)
				return std::to_string(current_seat - 1);

			previous_seat = current_seat;
		}

		throw std::runtime_error("No answer found");
	}
}
