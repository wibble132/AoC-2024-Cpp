#include "day02.h"
#include <set>
#include <boost/parser/parser.hpp>
#include <range/v3/view/sliding.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/indices.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/concat.hpp>
#include <range/v3/algorithm/count_if.hpp>

namespace
{
    std::vector<std::vector<int>> parse_input(const std::string& input)
    {
        namespace bp = boost::parser;
        auto parser = *((bp::int_ % ' ') >> bp::eol);
        std::optional<std::vector<std::vector<int>>> res = bp::parse(input, parser);
        if (!res.has_value())
        {
            throw std::runtime_error("Failed to parse input");
        }

        return res.value();
    }
}

constexpr std::array<int, 3> positive_success = {1, 2, 3};
constexpr std::array<int, 3> negative_success = {-3, -2, -1};

template <typename T>
bool is_line_valid(const T& input)
{
    // A line is valid if the differences of adjacent elements are either all positive or all negative,
    // and the absolute value of the differences is at least 1 and at most 3.

    auto x = input | ranges::views::sliding(2) | ranges::views::transform([](auto&& win)
    {
        auto it = win.begin();
        const int a = *it++;
        const int b = *it;
        return b - a;
    });
    std::set<int> y(x.begin(), x.end());

    return std::ranges::includes(positive_success, y) || std::ranges::includes(negative_success, y);
}

size_t Day02::part1() const
{
    auto input = parse_input(this->input);

    return ranges::count_if(input, [](auto& line) -> bool { return is_line_valid(line); });
}

template <typename T>
bool is_line_almost_valid(const T& input)
{
    const size_t size = std::ranges::size(input);
    for (size_t i = 0; i < size; ++i)
    {
        // Construct a view that skips the ith element of input

        auto first_i = input | std::ranges::views::take(i);
        auto after_first_i_1 = input | std::ranges::views::drop(i + 1);
        auto skipped = ranges::views::concat(first_i, after_first_i_1);
        if (is_line_valid(skipped))
            return true;
    }
    return false;
}


size_t Day02::part2() const
{
    auto input = parse_input(this->input);

    return ranges::count_if(input, [](auto& line) -> bool { return is_line_almost_valid(line); });
}
