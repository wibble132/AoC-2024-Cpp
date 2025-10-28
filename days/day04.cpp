#include "day04.h"

#include <string_view>
#include <vector>
#include <tuple>

#include <range/v3/view/sliding.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/algorithm/count_if.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/zip.hpp>

int Day04::part1() const
{
    // Look forwards, downwards, and both down-diagonals for either XMAS or SAMX

    // Build a grid of string_views for each line
    std::vector<std::string_view> grid{};
    grid.reserve(128);

    size_t start = 0;
    while (start < input.size())
    {
        size_t end = input.find('\n', start);
        if (end == std::string::npos) end = input.size();
        grid.emplace_back(input.data() + start, end - start);
        start = end + 1;
    }

    // Iterate through, looking for the patterns

    static constexpr auto is_match_base = [](const char c0, const char c1, const char c2, const char c3)
    {
        return (c0 == 'X' && c1 == 'M' && c2 == 'A' && c3 == 'S') ||
            (c0 == 'S' && c1 == 'A' && c2 == 'M' && c3 == 'X');
    };
    static constexpr auto is_match = [](auto... val)
    {
        using ArgsT = std::tuple<decltype(val)...>;
        if constexpr (requires { is_match_base(val...); })
        {
            return is_match_base(val...);
        }

        auto [v] = ArgsT(val...);
        if constexpr (requires { std::is_same_v<decltype(v[0]), const char&>; })
        {
            return is_match_base(v[0], v[1], v[2], v[3]);
        }
        else if constexpr (requires { std::is_same_v<decltype(v), std::tuple<char, char, char, char>>; })
        {
            return std::apply(is_match_base, v);
        }
    };


    // Horizontal matches
    // Iterate through rows,
    //  in each row take a sliding window checking for matches.
    int total = ranges::accumulate(grid | ranges::views::transform([](std::string_view line)
    {
        return ranges::count_if(line | ranges::views::sliding(4), is_match);
    }), 0);

    // Vertical matches
    // Sliding window of 4 rows,
    total += ranges::accumulate(grid | ranges::views::sliding(4) | ranges::views::transform([](auto&& win)
    {
        static_assert(std::is_same_v<decltype(win[0][0]), const char&>);
        // win is a list of 4 rows, want to iterate through them all at once
        auto it = ranges::view::zip(win[0], win[1], win[2], win[3]);
        return ranges::count_if(it, is_match);
    }), 0);

    return total;
}

int Day04::part2() const
{
    return 0;
}
