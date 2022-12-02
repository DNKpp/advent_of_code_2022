#include <fstream>
#include <ranges>
#include <string>
#include <charconv>
#include <filesystem>

#include <fmt/format.h>
#include <range/v3/all.hpp>

class getline_range
	: public ranges::view_facade<getline_range>
{
	friend ranges::range_access;

	std::string_view read() const { return m_Line; }

	bool equal(ranges::default_sentinel_t) const { return !m_IStream->good(); }

	void next() { std::getline(*m_IStream, m_Line); }

public:
	getline_range() = default;

	explicit getline_range(std::istream& in)
		: m_IStream{ std::addressof(in) }
	{
		next();
	}

private:
	std::istream* m_IStream;
	std::string m_Line{};
};

constexpr auto to_int = [](const std::string_view str)
{
	int temp{};
	std::from_chars(str.data(), str.data() + str.size(), temp);
	return temp;
};

void do_part1()
{
	std::ifstream in{ std::filesystem::path{ INPUT_DIR } / "input.txt" };
	int result{ 0 };
	while (in.good())
	{
		const int cur = ranges::accumulate(
			getline_range{ in }
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
	while (in.good())
	{
		const int cur = ranges::accumulate(
			getline_range{ in }
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
	std::ifstream in{ std::filesystem::path{ INPUT_DIR } / "input.txt" };
	std::vector<int> results(1000);
	while (in.good())
	{
		const int cur = ranges::accumulate(
			getline_range{ in }
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
