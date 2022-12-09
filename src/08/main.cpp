#include <cassert>
#include <deque>
#include <filesystem>
#include <fstream>
#include <ranges>
#include <map>

#include <ctre.hpp>
#include <fmt/format.h>
#include <range/v3/all.hpp>
#include <Simple-Utility/functional.hpp>
#include <experimental/mdarray>

#include "utility.hpp"

// solves https://adventofcode.com/2022/day/8

namespace sf = sl::functional;

template <class T>
using array2d = std::experimental::mdarray<T, std::experimental::dextents<int, 2>>;

std::vector<std::vector<int>> read_input(std::istream& in)
{
	return getline_range{ in }
			| ranges::views::transform(
				[](const std::string_view line)
				{
					return line
							| ranges::views::transform([](const char c) { return c - '0'; })
							| ranges::to_vector;
				}
			)
			| ranges::to_vector;
}

int do_part1(const std::string_view inputFileName)
{
	std::ifstream in{ std::filesystem::path{ INPUT_DIR } / inputFileName };
	const auto heightGrid = read_input(in);
	array2d<bool> visibilityGrid(heightGrid.size(), heightGrid.front().size());

	for (const int r : ranges::views::iota(0, visibilityGrid.extent(0)))
	{
		auto calcVis = [&](auto colSeq)
		{
			for (int max{ -1 }; const int c : colSeq)
			{
				if (const auto cur = heightGrid[r][c]; max < cur)
				{
					max = cur;
					visibilityGrid(r, c) = true;
				}
			}
		};

		calcVis(ranges::views::iota(0, visibilityGrid.extent(1)));
		calcVis(ranges::views::iota(0, visibilityGrid.extent(1)) | ranges::views::reverse);
	}

	for (const int c : ranges::views::iota(0, visibilityGrid.extent(1)))
	{
		auto calcVis = [&](auto rowSeq)
		{
			for (int max{ -1 }; const int r : rowSeq)
			{
				if (const auto cur = heightGrid[r][c]; max < cur)
				{
					max = cur;
					visibilityGrid(r, c) = true;
				}
			}
		};

		calcVis(ranges::views::iota(0, visibilityGrid.extent(0)));
		calcVis(ranges::views::iota(0, visibilityGrid.extent(0)) | ranges::views::reverse);
	}

	const int result = std::ranges::count(visibilityGrid.container(), true);

	fmt::print("answer for part 1 with file \"{}\" is: {}\n", inputFileName, result);

	return result;
}

int do_part2(const std::string_view inputFileName)
{
	std::ifstream in{ std::filesystem::path{ INPUT_DIR } / inputFileName };
	const auto heightGrid = read_input(in);
	const int height = std::ssize(heightGrid);
	const int width = std::ssize(heightGrid.front());

	const auto lookupScore = [&](const int r, const int c)
	{
		const int centerHeight = heightGrid[r][c];
		const sf::predicate_fn isLowerCenter = [&](const int curR, const int curC) { return heightGrid[curR][curC] < centerHeight; };

		const std::array values
		{
			std::min<std::int64_t>(
				c,
				1 + std::ranges::distance(
					ranges::views::iota(0, c) | ranges::views::reverse | ranges::views::take_while(isLowerCenter << r)
				)
			),
			std::min<std::int64_t>(
				width - 1 - c,
				1 + std::ranges::distance(ranges::views::iota(c + 1, width) | ranges::views::take_while(isLowerCenter << r))
			),
			std::min<std::int64_t>(
				r,
				1 + std::ranges::distance(
					ranges::views::iota(0, r) | ranges::views::reverse | ranges::views::take_while(isLowerCenter >> c)
				)
			),
			std::min<std::int64_t>(
				height - 1 - r,
				1 + std::ranges::distance(ranges::views::iota(r + 1, height) | ranges::views::take_while(isLowerCenter >> c))
			)
		};

		return ranges::accumulate(values, 1, std::multiplies{});
	};

	const int result = std::ranges::max(
		ranges::views::cartesian_product(ranges::views::iota(1, height - 1), ranges::views::iota(1, width - 1))
		| ranges::views::transform(sf::tuple::apply(lookupScore))
	);

	fmt::print("answer for part 2 with file \"{}\" is: {}\n", inputFileName, result);

	return result;
}

int main()
{
	const auto smallP1Result = do_part1("small_input.txt");
	assert(smallP1Result == 21);

	const auto smallP2Result = do_part2("small_input.txt");
	assert(smallP2Result == 8);

	do_part1("input.txt");
	do_part2("input.txt");

	return 0;
}
