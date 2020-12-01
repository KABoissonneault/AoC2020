#include "programs/programs.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <charconv>

#include <fmt/format.h>

namespace
{
	using solver_function = std::string(*)(std::istream&);
	struct program
	{
		solver_function part1;
		solver_function part2;
	};

	program const programs[] = {
		{}
		, KAB_AOC_PROGRAMS
	};

	int g_day;
	std::string g_input;

	void parse_args(char** argv)
	{
		while (*argv != nullptr)
		{
			char const* arg = *argv;
			if (strncmp(arg, "--", 2) == 0)
			{
				char const* value = arg + 2;
				if (strcmp(value, "file") == 0)
				{
					if(argv[1] == nullptr)
						throw std::runtime_error("Missing argument after '--file' flag");

					char const* filepath = argv[1];
					std::ifstream file_input(filepath);

					if (!file_input.is_open())
						throw std::runtime_error(fmt::format("Invalid filepath '{}' after '--file' flag", filepath));

					file_input.seekg(0, std::ios::end);
					g_input.resize(file_input.tellg());
					file_input.seekg(0);
					file_input.read(g_input.data(), g_input.size());

					argv += 2;
				}
				else
				{
					throw std::runtime_error(fmt::format("Unknown flag '{}'", value));
				}
			}
			else
			{
				if (argv[1] != nullptr)
					throw std::runtime_error(fmt::format("Unrecognized argument after 'day' argument: {}", (argv[1])));

				auto const result = std::from_chars(arg, arg + strlen(arg), g_day);
				if (result.ec != std::errc(0))
					throw std::runtime_error("'day' argument was not an integer");

				if (g_day >= std::size(programs))
					throw std::runtime_error(fmt::format("Program for day '{}' does not exist", g_day));

				argv += 1;
			}
		}

		if (g_day == 0)
			throw std::runtime_error("Missing or invalid 'day' argument");
	}
}

int main(int argc, char** argv)
{
	(void)argc;
	parse_args(argv + 1);

	if (g_input.empty())
		throw std::runtime_error("No input provided");

	program const& p = programs[g_day];

	std::stringstream ss;
	ss.write(g_input.data(), g_input.size());

	std::string const result1 = p.part1(ss);

	ss.seekg(0);

	std::string const result2 = p.part2(ss);

	printf("1: %s\n2: %s\n", result1.c_str(), result2.c_str());

	return 0;
}
