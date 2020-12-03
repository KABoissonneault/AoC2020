#include "day2.h"

#include <iostream>

#include <fmt/format.h>

namespace kab::day2
{
	namespace
	{
		struct password_policy
		{
			int minimum, maximum;
			char letter;
		};

		template<typename Callback>
		void for_each_line(std::istream& i, Callback&& cb)
		{
			std::string line;
			while (std::getline(i, line))
			{
				password_policy p;
				int read;
				(void)sscanf(line.data(), "%d-%d %c: %n", &p.minimum, &p.maximum, &p.letter, &read);

				std::string_view const password(line.c_str() + read);

				cb(p, password);
			}
		}
	}

	std::string solve_1(std::istream& i)
	{
		int valid = 0;
		for_each_line(i, [&valid](password_policy p, std::string_view password)
		{
			int occurence = 0;
			for (char c : password)
			{
				if (c == p.letter)
				{
					++occurence;
					if (occurence > p.maximum)
						break;
				}
			}

			if (occurence >= p.minimum && occurence <= p.maximum)
				++valid;
		});

		return std::to_string(valid);
	}

	std::string solve_2(std::istream& i)
	{
		int valid = 0;
		for_each_line(i, [&valid](password_policy p, std::string_view password)
		{
			if (password[p.minimum - 1] == p.letter && password[p.maximum - 1] != p.letter
				|| password[p.minimum - 1] != p.letter && password[p.maximum - 1] == p.letter)
				++valid;
		});

		return std::to_string(valid);
	}
}
