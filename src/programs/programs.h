#pragma once

#include <iosfwd>
#include <string>

namespace kab::day1 { std::string solve_1(std::istream& i); std::string solve_2(std::istream& i); }
namespace kab::day2 { std::string solve_1(std::istream& i); std::string solve_2(std::istream& i); }
namespace kab::day3 { std::string solve_1(std::istream& i); std::string solve_2(std::istream& i); }
namespace kab::day4 { std::string solve_1(std::istream& i); std::string solve_2(std::istream& i); }
namespace kab::day5 { std::string solve_1(std::istream& i); std::string solve_2(std::istream& i); }

#define KAB_AOC_PROGRAMS \
	{ kab::day1::solve_1, kab::day1::solve_2 }, \
	{ kab::day2::solve_1, kab::day2::solve_2 }, \
	{ kab::day3::solve_1, kab::day3::solve_2 }, \
	{ kab::day4::solve_1, kab::day4::solve_2 }, \
	{ kab::day5::solve_1, kab::day5::solve_2 }, \
	
