#include <fstream>
#include <filesystem>
#include <ranges>
#include <cassert>

#include <fmt/format.h>
#include <range/v3/all.hpp>

// solves https://adventofcode.com/2022/day/3

constexpr auto calc_score = [](const char c)
{
	return 'a' <= c
				? c - 'a' + 1
				: c - 'A' + 27;
};

void do_part1()
{
	std::ifstream in{ std::filesystem::path{ INPUT_DIR } / "input.txt" };

	const auto result = ranges::accumulate(
		ranges::istream<std::string>(in),
		0,
		std::plus{},
		[](const std::string_view rucksack)
		{
			const auto halfSize = std::ssize(rucksack) / 2;
			const auto iter = std::ranges::find_first_of(
				rucksack | std::views::take(halfSize),
				rucksack | std::views::drop(halfSize)
			);
			return calc_score(*iter);
		}
	);
	fmt::print("part 1 answer is: {}\n", result);
}

void do_part2()
{
	std::ifstream in{ std::filesystem::path{ INPUT_DIR } / "input.txt" };
	const std::vector input = ranges::istream<std::string>(in)
							| ranges::views::transform(
								[](std::string s) { return s |= ranges::actions::sort | ranges::actions::unique; }
							)
							| ranges::to_vector;

	const auto result = ranges::accumulate(
		ranges::views::zip_with(
			[](const std::string_view s1, const std::string_view s2, const std::string_view s3)
			{
				std::vector<char> oneSecInt{};
				std::ranges::set_intersection(s1, s2,std::back_inserter(oneSecInt));

				std::vector<char> finalInt{};
				std::ranges::set_intersection(oneSecInt, s3, std::back_inserter(finalInt));

				assert(std::ssize(finalInt) == 1);
				return calc_score(finalInt.front());
			},
			input | ranges::views::stride(3),
			input | ranges::views::drop(1) | ranges::views::stride(3),
			input | ranges::views::drop(2) | ranges::views::stride(3)
		),
		0
	);

	fmt::print("part 1 answer is: {}\n", result);
}

int main()
{
	do_part1();

	do_part2();

	return 0;
}
