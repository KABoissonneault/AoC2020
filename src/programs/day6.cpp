#include "programs.h"

#include <iostream>
#include <set>
#include <vector>
#include <algorithm>

namespace kab::day6
{
	std::string solve_1(std::istream& i)
	{
		int total = 0;

		std::set<char> group;

		std::string person;
		while (std::getline(i, person))
		{
			if (person.empty())
			{
				total += group.size();
				group.clear();
			}
			else
			{
				group.insert(person.begin(), person.end());
			}
		}

		if (!group.empty())
		{
			total += group.size();
		}

		return std::to_string(total);
	}

	namespace
	{
		std::set<char> get_questions()
		{
			std::set<char> s;
			auto [it, inserted] = s.emplace('a');
			for (char c = 'b'; c <= 'z'; ++c)
			{
				it = s.emplace_hint(it, c);
			}
			return s;
		}
	}

	std::string solve_2(std::istream& i)
	{
		int total = 0;
		std::set<char> questions = get_questions();

		std::string line;
		while (std::getline(i, line))
		{
			if (line.empty())
			{
				total += questions.size();
				questions = get_questions();
			}
			else
			{
				std::set<char> person(line.begin(), line.end());
				std::vector<char> intersection;
				std::ranges::set_intersection(questions, person, std::back_inserter(intersection));
				questions.clear();
				questions.insert(intersection.begin(), intersection.end());
			}
		}

		return std::to_string(total);
	}
}
