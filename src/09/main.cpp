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
#include <Simple-Vector/Vector.hpp>
#include <experimental/mdarray>

#include "utility.hpp"

// solves https://adventofcode.com/2022/day/9

namespace sf = sl::functional;

using vec = sl::vec::Vector<int, 2>;

std::vector<vec> parse_direction(std::string_view str)
{
	const char dirStr = str.front();
	str.remove_prefix(2);
	const int dist = to_int(str);

	const vec dir = [&]() -> vec
	{
		switch (dirStr)
		{
		case 'L': return { -1, 0 };
		case 'R': return { 1, 0 };
		case 'U': return { 0, -1 };
		case 'D': return { 0, 1 };
		}
		return { 0, 0 };
	}();

	return std::vector(dist, dir);
}

template <std::size_t VKnots>
int simulate(std::istream& in)
{
	std::array<vec, VKnots> segments{};
	std::ranges::fill(segments, vec{ 0, 0 });

	std::vector visitedPos{ vec{ 0, 0 } };

	for (const vec dir
		: getline_range{ in }
		| ranges::views::transform(&parse_direction)
		| ranges::views::join
	)
	{
		segments.front() += dir;

		for (auto&& [leading, trailing] : ranges::views::zip(
				segments | ranges::views::const_,
				segments | ranges::views::tail
			)
		)
		{
			if (auto diff = leading - trailing; 2 < length_squared(diff))
			{
				trailing += vec{ std::clamp(diff.x(), -1, 1), std::clamp(diff.y(), -1, 1) };
			}
		}

		if (visitedPos.back() != segments.back())
		{
			visitedPos.emplace_back(segments.back());
		}
	}

	visitedPos |= ranges::actions::sort(
			[](const vec& lhs, const vec& rhs) { return std::ranges::lexicographical_compare(lhs, rhs); }
		)
		| ranges::actions::unique;

	return std::ssize(visitedPos);
}

int do_part1(const std::string_view inputFileName)
{
	std::ifstream in{ std::filesystem::path{ INPUT_DIR } / inputFileName };

	const int result = simulate<2>(in);

	fmt::print("answer for part 1 with file \"{}\" is: {}\n", inputFileName, result);

	return result;
}

int do_part2(const std::string_view inputFileName)
{
	std::ifstream in{ std::filesystem::path{ INPUT_DIR } / inputFileName };

	const int result = simulate<10>(in);

	fmt::print("answer for part 2 with file \"{}\" is: {}\n", inputFileName, result);

	return result;
}

int main()
{
	const auto smallP1Result = do_part1("small_input.txt");
	assert(smallP1Result == 13);

	const auto smallP2Result = do_part2("small_input.txt");
	assert(smallP2Result == 1);

	const auto mediumP2Result = do_part2("medium_input.txt");
	assert(mediumP2Result == 36);

	do_part1("input.txt");
	do_part2("input.txt");

	return 0;
}
