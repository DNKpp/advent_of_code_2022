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

#include "utility.hpp"

// solves https://adventofcode.com/2022/day/7

namespace sf = sl::functional;

std::map<std::string, int> read_dirs(std::istream& in)
{
	std::vector<std::string> dirStack{};
	std::map<std::string, int> dirContentSizes{};

	static constexpr std::string_view cdPrefix = "$ cd ";
	static constexpr std::string_view lsPrefix = "$ ls";
	static constexpr std::string_view dirPrefix = "dir ";

	for (std::string_view cmd
		: getline_range{ in }
		| ranges::views::remove_if(
			[](const std::string_view s) { return s.starts_with(lsPrefix) || s.starts_with(dirPrefix); }
		)
	)
	{
		if (cmd.starts_with(cdPrefix))
		{
			cmd.remove_prefix(cdPrefix.size());
			if (cmd == "..")
			{
				dirStack.pop_back();
			}
			else
			{
				const std::string dirName = (!dirStack.empty() ? dirStack.back() + "/" : "")
											+ cmd.data();
				dirStack.emplace_back(dirName);
				auto [_, success] = dirContentSizes.emplace(dirName, 0);
				assert(success);
			}
		}
		else
		{
			const auto& [whole, size, name] = ctre::match<R"((\d+)\s(\w+.?\w*))">(cmd);
			dirStack
				| ranges::views::transform([&](const auto& s) -> int& { return dirContentSizes[s]; })
				| ranges::actions::transform(sf::transform_fn{ std::plus{} } >> to_int(size));
		}
	}

	return dirContentSizes;
}

int do_part1(const std::string_view inputFileName)
{
	std::ifstream in{ std::filesystem::path{ INPUT_DIR } / inputFileName };
	const auto dirContentSizes = read_dirs(in);

	const auto result = ranges::accumulate(
		dirContentSizes
		| ranges::views::values
		| ranges::views::filter(sf::less_equal >> 100'000),
		0
	);

	fmt::print("answer for part 1 with file \"{}\" is: {}\n", inputFileName, result);

	return result;
}

int do_part2(const std::string_view inputFileName)
{
	constexpr int totalSpace = 70'000'000;
	constexpr int requiredSpace = 30'000'000;

	std::ifstream in{ std::filesystem::path{ INPUT_DIR } / inputFileName };
	const auto dirContentSizes = read_dirs(in);
	const int unusedSpace = totalSpace - dirContentSizes.at("/");

	const auto result = std::ranges::min(
		dirContentSizes
		| ranges::views::values
		| ranges::views::filter(sf::greater_equal >> (requiredSpace - unusedSpace))
	);

	fmt::print("answer for part 2 with file \"{}\" is: {}\n", inputFileName, result);

	return result;
}

int main()
{
	const auto smallP1Result = do_part1("small_input.txt");
	assert(smallP1Result == 95437);

	const auto smallP2Result = do_part2("small_input.txt");
	assert(smallP2Result == 24933642);

	do_part1("input.txt");
	do_part2("input.txt");

	return 0;
}
