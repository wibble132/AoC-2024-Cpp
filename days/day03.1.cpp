#include "day03.1.h"
#include <boost/parser/parser.hpp>

// References:
// - https://www.boost.org/doc/libs/1_89_0/doc/html/boost_parser/cheat_sheet.html
// - https://www.boost.org/doc/libs/1_89_0/doc/html/boost_parser/extended_examples.html#boost_parser.extended_examples.parsing_json_with_callbacks

namespace parse1
{
    namespace bp = boost::parser;

    class mul_tag{};

    bp::callback_rule<mul_tag, std::tuple<unsigned int, unsigned int>> mul = "mul";

    using uint3_parser = bp::uint_parser<unsigned int, 10, 1, 3>;
    inline constexpr bp::parser_interface<uint3_parser> number_{};
    auto mul_def = "mul(" >> number_ >> ',' >> number_ >> ')';

    BOOST_PARSER_DEFINE_RULES(
        mul
    );
}

unsigned int Day03_1::part1() const
{
    namespace bp = boost::parser;

    struct callbacks
    {
        mutable unsigned int total = 0;

        void operator()(parse1::mul_tag, std::tuple<unsigned int, unsigned int> args) const
        {
            auto [a, b] = args;
            total += a * b;
        }
    };

    constexpr callbacks cb{};
    const bool success = bp::callback_parse(input, *(parse1::mul | bp::char_), cb);
    if (!success)
    {
        throw std::runtime_error("Day 3; Part 1: Parsing failed");
    }

    return cb.total;
}


namespace parse2
{
    namespace bp = boost::parser;

    class mul_tag{};
    class do_tag{};
    class dont_tag{};

    constexpr bp::callback_rule<mul_tag, std::tuple<unsigned int, unsigned int>> mul = "mul";
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

unsigned int Day03_1::part2() const
{
    namespace bp = boost::parser;

    struct callbacks
    {
        mutable bool enabled = true;
        mutable unsigned int total = 0;

        void operator()(parse2::mul_tag, std::tuple<unsigned int, unsigned int> args) const
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

    constexpr callbacks cb{};
    constexpr auto parser = *(parse2::mul | parse2::doo | parse2::dont | bp::char_);
    const bool success = bp::callback_parse(input, parser, cb);
    if (!success)
    {
        throw std::runtime_error("Day 3; Part 2; Parsing failed");
    }

    return cb.total;
}
