#include "day03.h"

#include <iostream>
#include <boost/parser/parser.hpp>

// References:
// - https://www.boost.org/doc/libs/1_89_0/doc/html/boost_parser/cheat_sheet.html
// - https://www.boost.org/doc/libs/1_89_0/doc/html/boost_parser/extended_examples.html#boost_parser.extended_examples.parsing_json_with_callbacks

namespace parse1
{
    namespace bp = boost::parser;

    class mul_tag
    {
    };

    bp::callback_rule<mul_tag, std::tuple<int, int>> mul = "mul";

    using uint3_parser = bp::uint_parser<unsigned int, 10, 1, 3>;
    inline constexpr bp::parser_interface<uint3_parser> number_{};
    auto mul_def = "mul(" >> number_ >> ',' >> number_ >> ')';

    BOOST_PARSER_DEFINE_RULES(
        mul
    );
}

int Day03::part1()
{
    namespace bp = boost::parser;

    struct callbacks
    {
        mutable int total = 0;

        void operator()(parse1::mul_tag, std::tuple<int, int> args) const
        {
            auto [a, b] = args;
            total += a * b;
        }
    };

    constexpr callbacks callbacks{};
    const bool success = bp::callback_parse(input, *(parse1::mul | bp::char_), callbacks);
    if (!success)
    {
        throw std::runtime_error("Parsing failed");
    }

    return callbacks.total;
}


namespace parse2
{
    namespace bp = boost::parser;

    class mul_tag
    {
    };

    class do_tag
    {
    };

    class dont_tag
    {
    };

    constexpr bp::callback_rule<mul_tag, std::tuple<int, int>> mul = "mul";
    constexpr bp::callback_rule<do_tag> doo = "do";
    constexpr bp::callback_rule<dont_tag> dont = "don't";

    using uint3_parser = bp::uint_parser<unsigned int, 10, 1, 3>;
    inline constexpr bp::parser_interface<uint3_parser> number_{};
    constexpr auto mul_def = "mul(" >> number_ >> ',' >> number_ >> ')';

    constexpr auto doo_def = bp::lit("do()");
    constexpr auto dont_def = bp::lit("don't()");

    BOOST_PARSER_DEFINE_RULES(
        mul,
        doo,
        dont,
    );
}

int Day03::part2()
{
    struct callbacks
    {
        mutable bool enabled = true;
        mutable int total = 0;

        void operator()(parse2::mul_tag, std::tuple<int, int> args) const
        {
            if (!enabled) return;

            auto [a, b] = args;
            total += a * b;
        }

        void operator()(parse2::do_tag) const
        {
            enabled = true;
        }

        void operator()(parse2::dont_tag) const
        {
            enabled = false;
        }
    };

    constexpr callbacks callbacks{};
    constexpr auto parser = *(parse2::mul | parse2::doo | parse2::dont | boost::parser::char_);
    const bool success = boost::parser::callback_parse(input, parser, callbacks);
    if (!success)
    {
        throw std::runtime_error("Parsing failed");
    }

    return callbacks.total;
}
