#include <fstream>
#include <ranges>
#include <string>
#include <charconv>
#include <filesystem>

#include <fmt/format.h>
#include <range/v3/all.hpp>

#include "utility.hpp"

void do_part1()
{
	std::ifstream in{std::filesystem::path{INPUT_DIR} / "input.txt"};
	int result{0};
	while (getline_range getline{in})
	{
		const int cur = ranges::accumulate(
			getline
			| ranges::views::take_while(std::not_fn(&std::string_view::empty))
			| std::views::transform(to_int),
			0
		);

		result = std::max(result, cur);
	}

	fmt::print("part 1 answer is: {}\n", result);
}

/*
void do_part2()
{
	std::ifstream in{ std::filesystem::path{ INPUT_DIR } / "input.txt" };
	std::array<int, 3> results{};
	while (getline_range getline{in})
	{
		const int cur = ranges::accumulate(
			getline
			| ranges::views::take_while(std::not_fn(&std::string_view::empty))
			| std::views::transform(to_int),
			0
		);

		if (auto itr = std::ranges::upper_bound(results, cur, std::greater{}); itr != results.end())
		{
			std::shift_right(itr, results.end(), 1);
			*itr = cur;
		}
	}

	fmt::print("part 2 answer is: {}\n", ranges::accumulate(results, 0));
}
*/

void do_part2()
{
	std::ifstream in{std::filesystem::path{INPUT_DIR} / "input.txt"};
	std::vector<int> results{};
	while (getline_range getline{in})
	{
		const int cur = ranges::accumulate(
			getline
			| ranges::views::take_while(std::not_fn(&std::string_view::empty))
			| std::views::transform(to_int),
			0
		);

		results.emplace_back(cur);
	}

	ranges::partial_sort(results, results.begin() + 3, std::greater{});
	fmt::print("part 2 answer is: {}\n", ranges::accumulate(results | std::views::take(3), 0));
}

int main()
{
	do_part1();

	do_part2();

	return 0;
}
