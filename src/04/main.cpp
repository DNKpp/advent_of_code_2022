#include <fstream>
#include <filesystem>
#include <ranges>
#include <cassert>
#include <regex>

#include <fmt/format.h>
#include <range/v3/all.hpp>
#include <Simple-Utility/functional.hpp>

#include "utility.hpp"

// solves https://adventofcode.com/2022/day/4

namespace sf = sl::functional;

struct bounds
{
	int begin;
	int end;

	[[nodiscard]]
	constexpr bool contains(const bounds& other) const noexcept
	{
		return begin <= other.begin && other.end <= end;
	}

	[[nodiscard]]
	constexpr bool overlapping(const bounds& other) const noexcept
	{
		return begin <= other.end && other.begin <= end;
	}
};

std::tuple<bounds, bounds> parse_bound_pair(const std::string_view str)
{
	static const std::regex pattern{ R"((\d+)-(\d+),(\d+)-(\d+))" };
	std::cmatch matchResult{};
	std::regex_match(str.data(), matchResult, pattern);
	auto matchedInts = matchResult
						| std::views::transform(&std::csub_match::str)
						| std::views::transform(to_int);
	return { bounds{ matchedInts[1], matchedInts[2] }, bounds{ matchedInts[3], matchedInts[4] } };
}

void do_part1()
{
	std::ifstream in{ std::filesystem::path{ INPUT_DIR } / "input.txt" };

	const auto result = std::ranges::count_if(
		getline_range{ in }
		| std::views::transform(parse_bound_pair),
		sf::tuple::apply(
			[](const bounds& lhs, const bounds& rhs) { return lhs.contains(rhs) || rhs.contains(lhs); }
		)
	);

	fmt::print("part 1 answer is: {}\n", result);
}

void do_part2()
{
	std::ifstream in{ std::filesystem::path{ INPUT_DIR } / "input.txt" };

	const auto result = std::ranges::count_if(
		getline_range{ in }
		| std::views::transform(parse_bound_pair),
		sf::tuple::apply(&bounds::overlapping)
	);

	fmt::print("part 2 answer is: {}\n", result);
}

int main()
{
	do_part1();

	do_part2();

	return 0;
}
