#include "day05.h"

#include <boost/parser/parser.hpp>
#include <range/v3/view/sliding.hpp>
#include <range/v3/algorithm/all_of.hpp>

#include <algorithm>
#include <tuple>
#include <vector>
#include <string>

std::tuple<std::vector<std::tuple<int, int>>, std::vector<std::vector<int>>> parse_data(const std::string &input)
{
    namespace bp = boost::parser;
    constexpr bp::parser_interface<bp::int_parser<int, 10, 2, 2>> int_;

    constexpr auto parser =
        *(int_ >> '|' >> int_ >> bp::eol) >> bp::eol >> *((int_ % ',') >> bp::eol);

    const auto res = bp::parse(input, parser);

    if (!res.has_value())
    {
        throw std::runtime_error("Failed to parse input");
    }

    return res.value();
}

bool is_pair_ok(const std::tuple<int, int> &pair, const std::vector<std::tuple<int, int>> &ordering_rules)
{
    return std::find(ordering_rules.begin(), ordering_rules.end(), pair) != ordering_rules.end();
}

bool is_ok(const std::vector<std::tuple<int, int>> &ordering_rules, const std::vector<int> &pages)
{
    // Would prefer this to be functional,
    // but don't know how to create an iterator of (1,2), (1,3), (1,4), ..., (2,3), (2,4), ...
    const auto len = pages.size();
    for (std::size_t i = 0; i < len - 1; ++i)
    {
        const auto p1 = pages[i];
        for (std::size_t j = i + 1; j < len; ++j)
        {
            const auto p2 = pages[j];
            if (!is_pair_ok(std::make_tuple(p1, p2), ordering_rules))
            {
                return false;
            }
        }
    }

    return true;
}

int Day05::part1() const
{
    const auto [ordering_rules, pages_list] = parse_data(this->input);

    int total = 0;
    for (const auto &pages : pages_list)
    {
        std::size_t len = pages.size();
        if (is_ok(ordering_rules, pages))
        {
            total += pages[len / 2];
        }
    }

    return total;
}

int Day05::part2() const
{
    auto [ordering_rules, pages_list] = parse_data(this->input);

    int total = 0;

    auto comparison = [&ordering_rules](const int &a, const int &b) {
        return is_pair_ok(std::make_tuple(a, b), ordering_rules);
    };

    for (auto &pages : pages_list)
    {
        // We only care about the non-sorted updates
        if (is_ok(ordering_rules, pages))
        {
            continue;
        }

        std::sort(pages.begin(), pages.end(), comparison);

        total += pages[pages.size() / 2];

        if (!is_ok(ordering_rules, pages))
        {
            throw std::runtime_error("Sorting failed to produce a valid order");
        }
    }

    return total;
}
