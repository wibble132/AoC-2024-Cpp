#include "day05.h"

#include <boost/parser/parser.hpp>
#include <range/v3/view/sliding.hpp>
#include <range/v3/algorithm/all_of.hpp>

#include <algorithm>
#include <tuple>
#include <vector>
#include <string>

std::tuple<std::vector<std::tuple<int, int>>, std::vector<std::vector<int>>> parse_data(const std::string& input)
{
    namespace bp = boost::parser;

    constexpr auto parser =
        *(bp::int_ >> '|' >> bp::int_ >> bp::eol)
        >> bp::eol
        >> *((bp::int_ % ',') >> bp::eol);

    const auto res = bp::parse(input, parser);

    if (!res.has_value())
    {
        throw std::runtime_error("Failed to parse input");
    }

    return res.value();
}

int Day05::part1() const
{
    const auto [ordering_rules, pages_list] = parse_data(this->input);

    int total = 0;
    for (const auto& pages : pages_list)
    {
        const bool ok = ranges::all_of(pages | ranges::views::sliding(2), [&ordering_rules](const auto& window) {
            const auto a = window[0];
            const auto b = window[1];

            // This doesn't seem to work?
            // const auto [a,b] = window;


            // If we find (b, a) among the rules, this pair violates ordering
            return std::find(ordering_rules.begin(), ordering_rules.end(), std::make_tuple(b, a)) == ordering_rules.end();
        });

        if (ok) total += 1;
    }

    return total;
}

int Day05::part2() const
{
    return 0;
}
