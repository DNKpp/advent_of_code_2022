#pragma once

#include <istream>
#include <string>

#include <range/v3/view/facade.hpp>

class getline_range
	: public ranges::view_facade<getline_range>
{
	friend ranges::range_access;

private:
	std::istream* m_IStream{};
	std::string m_Line{};
	bool m_HasValue{false};
	char m_Delimiter{ '\n' };

public:
	getline_range() = default;

	explicit getline_range(std::istream& in)
		: m_IStream{ &in }
	{
		next();
	}

	explicit getline_range(std::istream& in, const char delimiter)
		: m_IStream{ &in },
		m_Delimiter{ delimiter }
	{
		next();
	}

	[[nodiscard]]
	bool empty() const { return !m_HasValue; }

private:
	[[nodiscard]]
	std::string_view read() const noexcept { return m_Line; }

	[[nodiscard]]
	bool equal(ranges::default_sentinel_t) const { return empty(); }

	void next()
	{
		m_HasValue = !!std::getline(*m_IStream, m_Line, m_Delimiter);
	}
};

constexpr auto to_int = [](const std::string_view str)
{
	int temp{};
	std::from_chars(str.data(), str.data() + str.size(), temp);
	return temp;
};
