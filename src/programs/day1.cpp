#include "day1.h"

#include <iostream>
#include <charconv>
#include <unordered_map>
#include <tuple>

#include <fmt/format.h>

namespace kab::day1
{
	std::string solve_1(std::istream& i)
	{
		// Key: complement
		// Value: original value
		std::unordered_map<int, int> m;

		std::string arg;
		while (std::getline(i, arg))
		{
			int value;
			auto const result = std::from_chars(arg.data(), arg.data() + arg.size(), value);
			if (result.ec != std::errc(0))
				throw std::runtime_error(fmt::format("Unexpected non-integer in input: %s", arg));

			// Find if the value is the complement of a previous input
			auto const it_existing = m.find(value);			
			if (it_existing != m.end())
			{
				return std::to_string(value * it_existing->second);
			}
			else
			{
				int const complement = 2020 - value;
				m.emplace(complement, value);
			}
		}

		throw std::runtime_error("Could not find solution.");
	}

	std::string solve_2(std::istream& i)
	{
		// Key: complement
		// Value: the two input values that need this complement
		std::unordered_map<int, std::tuple<int, int>> m;
		std::vector<int> previous_input;

		std::string arg;
		while (std::getline(i, arg))
		{
			int value;
			auto const result = std::from_chars(arg.data(), arg.data() + arg.size(), value);
			if (result.ec != std::errc(0))
				throw std::runtime_error(fmt::format("Unexpected non-integer in input: %s", arg));

			// Find if the value is the complement of a previous input
			auto const it_existing = m.find(value);
			if (it_existing != m.end())
			{
				auto const& input_pair = it_existing->second;
				return std::to_string(value * std::get<0>(input_pair) * std::get<1>(input_pair));
			}
			else
			{
				for (int previous_value : previous_input)
				{
					if (value + previous_value < 2020)
					{
						int const complement = 2020 - (value + previous_value);
						m.emplace(complement, std::make_tuple(value, previous_value));
					}
				}

				previous_input.push_back(value);
			}
		}

		throw std::runtime_error("Could not find solution.");
	}
}
