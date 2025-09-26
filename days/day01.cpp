#include "day01.h"
#include <boost/parser/parser.hpp>
#include <ranges>
#include <range/v3/view/zip.hpp>

namespace
{
    std::vector<std::tuple<int, std::string, int>> parse_input(const std::string& input)
    {
        auto p = *(boost::parser::int_ >> boost::parser::string("   ") >> boost::parser::int_ >> boost::parser::eol);
        std::optional<std::vector<std::tuple<int, std::string, int>>> res = boost::parser::parse(input, p);

        if (!res.has_value())
        {
            throw std::runtime_error("Failed to parse input");
        }

        return std::move(res.value());
    }
}

int Day01::part1()
{
    const auto input = parse_input(this->input);

    // Views over the first and third elements (left and right columns)
    auto left_ids_view = input | std::views::transform([](const auto& t) { return std::get<0>(t); });
    auto right_ids_view = input | std::views::transform([](const auto& t) { return std::get<2>(t); });

    // Materialize into vectors (lists)
    std::vector<int> left_list(left_ids_view.begin(), left_ids_view.end());
    std::vector<int> right_list(right_ids_view.begin(), right_ids_view.end());

    std::ranges::sort(left_list);
    std::ranges::sort(right_list);

    // zip the lists together
    const auto x =
        ranges::zip_view(left_list, right_list)
        | std::ranges::views::transform(
            [](const auto& pair) { return std::abs(std::get<0>(pair) - std::get<1>(pair)); });
    const int sum = std::accumulate(x.begin(), x.end(), 0);

    return sum;
}

int Day01::part2()
{
    const auto input = parse_input(this->input);

    std::map<int, int> left_counts;
    std::map<int, int> right_counts;
    for (const auto& [left, _, right] : input)
    {
        left_counts[left]++;
        right_counts[right]++;
    }
    auto left_iter = left_counts.begin();
    auto right_iter = right_counts.begin();

    int total = 0;
    while (left_iter != left_counts.end() && right_iter != right_counts.end())
    {
        if (left_iter->first == right_iter->first)
        {
            total += left_iter->second * right_iter->second * left_iter->first;
            ++left_iter;
            ++right_iter;
        }
        else if (left_iter->first < right_iter->first)
            ++left_iter;
        else
            ++right_iter;
    }

    return total;
}
