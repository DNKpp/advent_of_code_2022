#include <fstream>
#include <filesystem>
#include <ranges>
#include <cassert>
#include <regex>
#include <deque>

#include <fmt/format.h>
#include <range/v3/all.hpp>
#include <Simple-Utility/functional.hpp>

#include "utility.hpp"

// solves https://adventofcode.com/2022/day/6

namespace sf = sl::functional;

int check_message(const std::string_view msg, const int windowSize)
{
	const auto r = msg | ranges::views::sliding(windowSize);
	const auto iter = std::ranges::find_if(
		r,
		[](const auto& chars)
		{
			auto right = ranges::views::iota(1, std::ssize(chars))
						| ranges::views::transform([&](const auto i) { return chars | ranges::views::drop(i); });

			return std::ranges::all_of(
				ranges::views::zip(chars, right),
				sf::tuple::apply([](const char c, const auto& r) { return std::ranges::none_of(r, sf::equal << c); })
			);
		}
	);

	return static_cast<int>(std::ranges::distance(r.begin(), iter)) + windowSize;
}

std::string read_input(const std::string_view inputFileName)
{
	std::ifstream in{ std::filesystem::path{ INPUT_DIR } / inputFileName };
	std::string buffer{};
	in >> buffer;
	return buffer;
}

int do_part1(const std::string_view inputFileName)
{
	const std::string input = read_input(inputFileName);

	const auto result = check_message(input, 4);

	fmt::print("answer for part 1 with file \"{}\" is: {}\n", inputFileName, result);

	return result;
}

int do_part2(const std::string_view inputFileName)
{
	const std::string input = read_input(inputFileName);

	const auto result = check_message(input, 14);

	fmt::print("answer for part 1 with file \"{}\" is: {}\n", inputFileName, result);

	return result;
}

int main()
{
	const auto smallP1Result = do_part1("small_input.txt");
	assert(smallP1Result == 7);

	const auto smallP2Result = do_part2("small_input.txt");
	assert(smallP2Result == 19);

	do_part1("input.txt");
	do_part2("input.txt");

	return 0;
}
