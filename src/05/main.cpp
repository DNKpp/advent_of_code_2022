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

// solves https://adventofcode.com/2022/day/5

namespace sf = sl::functional;

static constexpr int columnWidth = 4;

std::vector<std::deque<char>> parse_container_begin_state(std::istream& in)
{
    std::vector<std::deque<char>> state{};

    for (const std::string_view line
        : getline_range{ in }
        | std::views::take_while([](const std::string_view str) { return !str.starts_with(" 1 "); })
    )
    {
        state.resize((std::ssize(line) + 1) / columnWidth);

        for (auto&& [column, c] :
            line
            | ranges::views::drop(1)
            | ranges::views::stride(columnWidth)
            | ranges::views::enumerate
            | ranges::views::remove_if(sf::tuple::get_at<1> | isspace)
        )
        {
            state[column].push_front(c);
        }
    }

    return state;
}

struct instruction
{
	int amount;
	int from;
	int to;
};

instruction parse_instruction(const std::string_view str)
{
	static const std::regex pattern{ R"(move (\d+) from (\d+) to (\d+))" };
	std::cmatch matchResult{};
	std::regex_match(str.data(), matchResult, pattern);
	auto matchedInts = matchResult
						| std::views::transform(&std::csub_match::str)
						| std::views::transform(to_int);

	return { matchedInts[1], matchedInts[2], matchedInts[3] };
}

std::string do_part1(const std::string_view inputFileName)
{
	std::ifstream in{ std::filesystem::path{ INPUT_DIR } / inputFileName };
	auto state = parse_container_begin_state(in);

	for (auto&& [amount, from, to]
		: getline_range{ in }
		| std::views::drop(1)
		| std::views::transform(parse_instruction)
		| std::views::transform(
			[&](const instruction inst) -> std::tuple<int, std::deque<char>&, std::deque<char>&>
			{
				return { inst.amount, state[inst.from - 1], state[inst.to - 1] };
			}
		)
	)
	{
		from.erase(
			std::ranges::copy_n(
				std::views::reverse(from).begin(),
				amount,
				std::back_inserter(to)
			).in.base(),
			from.end()
		);
	}

	std::string result = state
						| std::views::transform([](const auto& c) { return c.back(); })
						| ranges::to<std::string>();

	fmt::print("answer for part 1 with file \"{}\" is: {}\n", inputFileName, result);

	return result;
}

std::string do_part2(const std::string_view inputFileName)
{
	std::ifstream in{ std::filesystem::path{ INPUT_DIR } / inputFileName };
	auto state = parse_container_begin_state(in);

	for (auto&& [amount, from, to]
		: getline_range{ in }
		| std::views::drop(1)
		| std::views::transform(parse_instruction)
		| std::views::transform(
			[&](const instruction inst) -> std::tuple<int, std::deque<char>&, std::deque<char>&>
			{
				return { inst.amount, state[inst.from - 1], state[inst.to - 1] };
			}
		)
	)
	{
		std::ranges::copy(
			from | ranges::views::take_last(amount),
			std::back_inserter(to)
		);
		from.resize(std::ssize(from) - amount);
	}

	std::string result = state
						| std::views::transform([](const auto& c) { return c.back(); })
						| ranges::to<std::string>();

	fmt::print("answer for part 1 with file \"{}\" is: {}\n", inputFileName, result);

	return result;
}

int main()
{
	const auto smallP1Result = do_part1("small_input.txt");
	assert(smallP1Result == "CMZ");

	const auto smallP2Result = do_part2("small_input.txt");
	assert(smallP2Result == "MCD");

	do_part1("input.txt");
	do_part2("input.txt");

	return 0;
}
