#include "programs.h"

#include <iostream>
#include <set>
#include <string_view>
#include <vector>
#include <charconv>
#include <optional>
#include <span>

#include <fmt/format.h>

namespace kab::day7
{
	std::set<std::string> color_cache;

	namespace
	{
		struct rule_item
		{
			int count;
			std::string_view color;
		};

		struct rule
		{
			std::string_view color;
			std::vector<rule_item> items;
		};

		bool is_whitespace(char c)
		{
			return std::isspace(c) || c == ',' || c == '.';
		}

		size_t next_whitespace(std::string_view s)
		{
			auto const it_token = std::ranges::find_if(s, [](char c)
			{
				return is_whitespace(c);
			});

			return std::distance(s.begin(), it_token);
		}

		size_t next_non_whitespace(std::string_view s)
		{
			auto const it_token = std::ranges::find_if(s, [](char c)
			{
				return !is_whitespace(c);
			});

			return std::distance(s.begin(), it_token);
		}

		std::string_view left_trim(std::string_view s)
		{
			size_t const whitespace_size = next_non_whitespace(s);
			s.remove_prefix(whitespace_size);
			return s;
		}

		std::string_view parse_token(std::string_view& line)
		{
			line = left_trim(line);
			size_t token_end = next_whitespace(line);
			std::string_view const token(line.data(), token_end);
			line.remove_prefix(token.size());
			return token;
		}

		int parse_int(std::string_view& line)
		{
			std::string_view const token = parse_token(line);
			int value;
			std::from_chars(token.data(), token.data() + token.size(), value);
			return value;
		}

		std::string_view parse_color(std::string_view& line)
		{
			std::string_view const qualifier = parse_token(line);
			std::string_view const hue = parse_token(line);
			auto [it, inserted] = color_cache.insert(fmt::format("{} {}", qualifier, hue));
			(void)inserted;
			return *it;
		}

		rule_item parse_rule_item(std::string_view& line)
		{
			int const count = parse_int(line);
			std::string_view const color = parse_color(line);

			std::string_view const bag_identifier = parse_token(line);
			if (bag_identifier != "bag" && bag_identifier != "bags") throw std::runtime_error("Invalid input");
			return { count, color };
		}

		std::vector<rule_item> parse_rule_items(std::string_view& line)
		{
			if (line.substr(0, 14) == " no other bags")
			{
				line.remove_prefix(14);
				return {};
			}
			else
			{
				std::vector<rule_item> rule_items;
				do
				{
					rule_items.push_back(parse_rule_item(line));
				} while (line.front() == ',');
				return rule_items;
			}			
		}

		rule parse_rule(std::string_view line)
		{
			rule r;

			r.color = parse_color(line);
			if (parse_token(line) != "bags") throw std::runtime_error("Invalid input");
			if (parse_token(line) != "contain") throw std::runtime_error("Invalid input");
			r.items = parse_rule_items(line);

			return r;
		}

		std::vector<rule> parse_rules(std::istream& i)
		{
			std::vector<rule> rules;

			std::string line;
			while (std::getline(i, line) && !line.empty())
			{
				rules.push_back(parse_rule(line));
			}

			return rules;
		}
	}

	std::string solve_1(std::istream& i)
	{
		std::vector<rule> const rules = parse_rules(i);

		std::set<std::string_view> valid_colors;
		int previous_count;
		do
		{
			previous_count = valid_colors.size();
			for (rule const& r : rules)
			{
				for (rule_item const& item : r.items)
				{
					if (item.color == "shiny gold" || valid_colors.count(item.color) != 0)
					{
						valid_colors.insert(r.color);
						break;
					}
				}
			}
		} while (valid_colors.size() != previous_count);

		return std::to_string(valid_colors.size());
	}

	namespace
	{
		int count_bags(rule const& r, std::span<rule const> sorted_rules)
		{
			int bag_count = 0;
			for (rule_item const& i : r.items)
			{
				rule const& item_rule = *std::ranges::lower_bound(sorted_rules, i.color, {}, &rule::color);
				bag_count += i.count * count_bags(item_rule, sorted_rules) + i.count;
			}
			return bag_count;
		}
	}

	std::string solve_2(std::istream& i)
	{
		std::vector<rule> rules = parse_rules(i);
		std::ranges::sort(rules, {}, &rule::color);

		rule const& shiny_gold = *std::ranges::find(rules, "shiny gold", &rule::color);

		int const bag_count = count_bags(shiny_gold, rules);

		return std::to_string(bag_count);
	}
}
