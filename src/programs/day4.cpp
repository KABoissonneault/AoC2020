#include "programs.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <charconv>
#include <iterator>
#include <cctype>

namespace kab::day4
{
	struct key_value_pair
	{
		std::string key;
		std::string value;
	};

	using validation_function = bool(std::string_view);

	struct field
	{
		std::string_view key;
		validation_function* validation;
	};

	bool validate_byr(std::string_view value)
	{
		if (value.size() != 4)
			return false;

		int integer_value;
		std::from_chars(value.data(), value.data() + value.size(), integer_value);
		return integer_value >= 1920 && integer_value <= 2002;
	}

	bool validate_iyr(std::string_view value)
	{
		if (value.size() != 4)
			return false;

		int integer_value;
		std::from_chars(value.data(), value.data() + value.size(), integer_value);
		return integer_value >= 2010 && integer_value <= 2020;
	}

	bool validate_eyr(std::string_view value)
	{
		if (value.size() != 4)
			return false;

		int integer_value;
		std::from_chars(value.data(), value.data() + value.size(), integer_value);
		return integer_value >= 2020 && integer_value <= 2030;
	}

	bool validate_hgt(std::string_view value)
	{
		int integer_value;
		auto const result = std::from_chars(value.data(), value.data() + value.size(), integer_value);
		
		std::string_view const type(result.ptr, value.size() - std::distance(value.data(), result.ptr));
		if (type == "cm")
		{
			return integer_value >= 150 && integer_value <= 193;
		}
		else if (type == "in")
		{
			return integer_value >= 59 && integer_value <= 76;
		}
		else
		{
			return false;
		}
	}

	bool validate_hcl(std::string_view value)
	{
		if (value.size() != 7 || value.front() != '#')
			return false;

		auto const is_hex_char = [](char c) { return std::isdigit(c) || (c >= 'a' && c <= 'f'); };
		return std::all_of(value.begin() + 1, value.end(), is_hex_char);
	}

	bool validate_ecl(std::string_view value)
	{
		auto const eye_colors = { "amb", "blu", "brn", "gry", "grn", "hzl", "oth" };
		return std::ranges::find(eye_colors, value) != eye_colors.end();
	}

	bool validate_pid(std::string_view value)
	{
		auto const is_digit = [](char c) { return std::isdigit(c); };
		return value.size() == 9 && std::ranges::all_of(value, is_digit);
	}

	field const mandatory_fields[] =
	{ 
		{"byr", validate_byr}
		, {"iyr", validate_iyr}
		, {"eyr", validate_eyr}
		, {"hgt", validate_hgt}
		, {"hcl", validate_hcl}
		, {"ecl", validate_ecl}
		, {"pid", validate_pid}
	};

	void add_kvps(std::string_view line, std::vector<key_value_pair>& kvps)
	{
		size_t key_start = 0;
		for (size_t key_end = line.find(':', key_start); key_end != std::string::npos; key_end = line.find(':', key_start))
		{
			size_t const key_count = key_end - key_start;
			std::string key(line.substr(key_start, key_count));
			size_t const value_start = key_end + 1;
			size_t const value_end = line.find_first_of(" \r\n\t", value_start);
			size_t const value_count = value_end != std::string::npos ? value_end - value_start : line.size() - value_start;
			std::string value(line.substr(value_start, value_count));

			kvps.emplace_back(std::move(key), std::move(value));

			if (value_end == std::string::npos)
				break;

			key_start = value_end + 1;
		}
	}

	std::string solve_1(std::istream& i)
	{
		int valid_count = 0;
		auto const test_valid = [&valid_count] (std::vector<key_value_pair> const& kvps)
		{
			auto const is_key_in_kvps = [&kvps](std::string_view key)
			{
				return std::ranges::find(kvps, key, &key_value_pair::key) != kvps.end();
			};

			if (kvps.size() >= std::size(mandatory_fields) && std::ranges::all_of(mandatory_fields, is_key_in_kvps, &field::key))
				++valid_count;
		};

		std::vector<key_value_pair> kvps;

		std::string line;
		while (std::getline(i, line))
		{
			if (line.empty())
			{
				test_valid(kvps);
				kvps.clear();

				continue;
			}

			add_kvps(line, kvps);		
		}
		
		if (!kvps.empty())
			test_valid(kvps);

		return std::to_string(valid_count);
	}

	std::string solve_2(std::istream& i)
	{
		int valid_count = 0;
		auto const test_valid = [&valid_count](std::vector<key_value_pair> const& kvps)
		{
			auto const is_field_valid = [&kvps](field const& f)
			{
				auto const it_kvp = std::ranges::find(kvps, f.key, &key_value_pair::key);
				if (it_kvp == kvps.end())
					return false;

				return f.validation(it_kvp->value);
			};

			if (kvps.size() >= std::size(mandatory_fields) && std::ranges::all_of(mandatory_fields, is_field_valid))
				++valid_count;
		};

		std::vector<key_value_pair> kvps;

		std::string line;
		while (std::getline(i, line))
		{
			if (line.empty())
			{
				test_valid(kvps);
				kvps.clear();

				continue;
			}

			add_kvps(line, kvps);
		}

		if (!kvps.empty())
			test_valid(kvps);

		return std::to_string(valid_count);
	}
}
