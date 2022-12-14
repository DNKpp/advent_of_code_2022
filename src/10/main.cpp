#include <cassert>
#include <deque>
#include <filesystem>
#include <fstream>
#include <ranges>
#include <map>
#include <variant>

#include <ctre.hpp>
#include <fmt/format.h>
#include <range/v3/all.hpp>
#include <Simple-Utility/functional.hpp>
#include <Simple-Vector/Vector.hpp>
#include <experimental/mdarray>

#include "utility.hpp"

// solves https://adventofcode.com/2022/day/10

namespace sf = sl::functional;
namespace rv = ranges::views;

int do_part1(const std::string_view inputFileName)
{
	std::ifstream in{ std::filesystem::path{ INPUT_DIR } / inputFileName };

	auto parse_instruction = [reg{ 1 }](std::string_view str) mutable
	{
		if (str.starts_with("addx"))
		{
			str.remove_prefix(5);
			auto view = rv::repeat_n(reg, 2);
			reg += to_int(str);

			return view;
		}

		return rv::repeat_n(reg, 1);
	};

	const int result = ranges::accumulate(
		getline_range{ in }
		| rv::transform(std::ref(parse_instruction))
		| rv::join
		| rv::enumerate
		| rv::transform(sf::tuple::apply([](const auto cycle, const int value) { return std::tuple{ cycle + 1, value }; }))
		| rv::filter(sf::tuple::get_at<0> | [](const auto cycle) { return (cycle + 20) % 40 == 0; }),
		0,
		std::plus{},
		sf::tuple::apply([](const auto cycle, const int value) { return cycle * value; })
	);

	fmt::print("answer for part 1 with file \"{}\" is: {}\n", inputFileName, result);

	return result;
}

auto do_part2(const std::string_view inputFileName)
{
	constexpr int spriteWidth = 3;

	std::ifstream in{ std::filesystem::path{ INPUT_DIR } / inputFileName };

	std::experimental::mdarray<char, std::experimental::extents<int, 6, 40>> screen{};

	auto parse_instruction = [reg{ 1 }](std::string_view str) mutable
	{
		if (str.starts_with("addx"))
		{
			str.remove_prefix(5);
			auto view = rv::repeat_n(reg, 2);
			reg += to_int(str);

			return view;
		}

		return rv::repeat_n(reg, 1);
	};

	for (auto [cycle, value] : getline_range{ in }
								| rv::transform(std::ref(parse_instruction))
								| rv::join
								| rv::enumerate
	)
	{
		const int column = static_cast<int>(cycle) % screen.extent(1);
		const bool visible = std::abs(value - column) <= 1;
		screen(cycle / screen.extent(1), cycle % screen.extent(1)) = visible ? '#' : '.';
	}

	fmt::print("answer for part 2 with file \"{}\" is:\n", inputFileName);
	for (const int r : rv::iota(0, screen.extent(0)))
	{
		for (const int c : rv::iota(0, screen.extent(1)))
		{
			fmt::print("{}", screen(r, c));
		}

		fmt::print("\n");
	}

	return screen;
}

int main()
{
	const auto smallP1Result = do_part1("small_input.txt");
	assert(smallP1Result == 13140);

	const auto smallP2Result = do_part2("small_input.txt");

	do_part1("input.txt");
	do_part2("input.txt");

	return 0;
}
