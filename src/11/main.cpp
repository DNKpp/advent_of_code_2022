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

// solves https://adventofcode.com/2022/day/11

namespace sf = sl::functional;
namespace rv = ranges::views;

using item_lvl = std::uint64_t;
using item_op = std::function<item_lvl(item_lvl)>;

item_op parse_operation(std::istream& in)
{
	std::string buffer{};
	std::getline(in, buffer);
	auto [whole, op, valueStr] = ctre::search<R"(Operation: new = old ([*+]) ([\d\w]+))">(buffer);

	if (valueStr == "old")
	{
		if (op == "+")
		{
			return [](const item_lvl v) { return v + v; };
		}

		return [](const item_lvl v) { return v * v; };
	}

	if (op == "+")
	{
		return std::bind_front(std::plus{}, to_int(valueStr));
	}

	return std::bind_front(std::multiplies{}, to_int(valueStr));
}

struct monkey
{
	std::deque<item_lvl> items{};
	item_op operation{};
	item_lvl testValue{};
	int trueTarget{};
	int falseTarget{};

	friend std::istream& operator >>(std::istream& in, monkey& m)
	{
		std::string buffer{};
		std::getline(in, buffer);
		std::ranges::transform(
			ctre::range<R"(\d+)">(buffer),
			std::back_inserter(m.items),
			to_int
		);

		m.operation = parse_operation(in);

		std::getline(in, buffer);
		m.testValue = to_int(ctre::search<R"(Test: divisible by (\d+))">(buffer).get<1>());

		std::getline(in, buffer);
		m.trueTarget = to_int(ctre::search<R"(If true: throw to monkey (\d+))">(buffer).get<1>());

		std::getline(in, buffer);
		m.falseTarget = to_int(ctre::search<R"(If false: throw to monkey (\d+))">(buffer).get<1>());

		return in;
	}
};

std::vector<monkey> parse_monkeys(std::istream& in)
{
	std::vector<monkey> monkeys{};

	while (in.good())
	{
		std::string buffer{};

		// skip index line
		std::getline(in, buffer);

		in >> monkeys.emplace_back();

		// skip blank line
		std::getline(in, buffer);
	}

	return monkeys;
}

template <int VTurns, std::int64_t VDiv>
std::int64_t simulate_monkeys(std::vector<monkey> monkeys, const std::int64_t worryMod = std::numeric_limits<std::int64_t>::max())
{
	std::vector<std::int64_t> inspectionCounts(monkeys.size());
	for (auto [turn, data] : rv::cartesian_product(rv::iota(0, VTurns), rv::zip(monkeys, inspectionCounts)))
	{
		auto& [monkey, inspections] = data;
		for (const auto item : monkey.items)
		{
			item_lvl lvl = (monkey.operation(item) % worryMod) / VDiv;
			const int targetIndex = lvl % monkey.testValue == 0
										? monkey.trueTarget
										: monkey.falseTarget;
			monkeys[targetIndex].items.push_back(lvl);
		}

		inspections += std::size(monkey.items);
		monkey.items.clear();
	}

	std::ranges::nth_element(
		inspectionCounts,
		inspectionCounts.begin() + 1,
		std::greater{}
	);

	return ranges::accumulate(
		inspectionCounts | rv::take(2),
		1ll,
		std::multiplies{}
	);
}

auto do_part1(const std::string_view inputFileName)
{
	std::ifstream in{ std::filesystem::path{ INPUT_DIR } / inputFileName };

	const auto result = simulate_monkeys<20, 3>(parse_monkeys(in));

	fmt::print("answer for part 1 with file \"{}\" is: {}\n", inputFileName, result);

	return result;
}

auto do_part2(const std::string_view inputFileName)
{
	std::ifstream in{ std::filesystem::path{ INPUT_DIR } / inputFileName };

	std::vector monkeys = parse_monkeys(in);
	const std::int64_t worryMod = ranges::accumulate(monkeys, 1, std::multiplies{}, &monkey::testValue);
	const auto result = simulate_monkeys<10'000, 1>(std::move(monkeys), worryMod);

	fmt::print("answer for part 2 with file \"{}\" is: {}\n", inputFileName, result);

	return result;
}

int main()
{
	const auto smallP1Result = do_part1("small_input.txt");
	assert(smallP1Result == 10605);

	const auto smallP2Result = do_part2("small_input.txt");
	assert(smallP2Result == 2'713'310'158);

	do_part1("input.txt");
	do_part2("input.txt");

	return 0;
}
