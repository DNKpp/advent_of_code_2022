#include <fstream>
#include <filesystem>
#include <vector>
#include <cassert>

#include <fmt/format.h>
#include <range/v3/all.hpp>

enum class sign
{
	rock = 0,
	paper = 1,
	scissor = 2
};

enum class match_result
{
	lose = -1,
	draw = 0,
	win = 1
};

constexpr auto char_to_sign = [](const char c)
{
	if ('A' <= c && c <= 'C')
	{
		return static_cast<sign>(c - 'A');
	}

	assert('X' <= c && c <= 'Z');
	return static_cast<sign>(c - 'X');
};

constexpr auto char_to_result = [](const char c)
{
	assert('X' <= c && c <= 'Z');
	return static_cast<match_result>(c - 'X' - 1);
};

constexpr auto calc_match_score = [](const sign lhs, const sign rhs)
{
	const int matchValue = (static_cast<int>(rhs) + 4 - static_cast<int>(lhs)) % 3;
	return 3 * matchValue + static_cast<int>(rhs) + 1;
};

constexpr auto calc_match_score_from_result = [](const sign lhs, const match_result result)
{
	const int signScore = (static_cast<int>(lhs) + static_cast<int>(result) + 3) % 3 + 1;
	return (static_cast<int>(result) + 1) * 3 + signScore;
};

constexpr auto cached_input = []
{
	std::ifstream in{ std::filesystem::path{ INPUT_DIR } / "input.txt" };
	return std::ranges::subrange{ std::istream_iterator<char>{ in }, std::istream_iterator<char>{} }
			| ranges::to_vector;
};

void do_part1()
{
	const std::vector input = cached_input();

	const auto result = ranges::accumulate(
		input
		| ranges::views::chunk(2)
		| ranges::views::transform(
			[](const auto& subview)
			{
				return calc_match_score(char_to_sign(subview.front()), char_to_sign(subview[1]));
			}
		),
		0
	);

	fmt::print("part 1 answer is: {}\n", result);
}

void do_part2()
{
	const std::vector input = cached_input();

	const auto result = ranges::accumulate(
		input
		| ranges::views::chunk(2)
		| ranges::views::transform(
			[](const auto& subview)
			{
				return calc_match_score_from_result(char_to_sign(subview.front()), char_to_result(subview[1]));
			}
		),
		0
	);

	fmt::print("part 2 answer is: {}\n", result);
}

int main()
{
	do_part1();

	do_part2();

	return 0;
}
