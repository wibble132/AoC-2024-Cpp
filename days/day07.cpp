#include "day07.h"

#include <valarray>
#include <vector>
#include <boost/parser/parser.hpp>

struct Equation
{
    uint64_t result;
    std::vector<uint64_t> inputs;

    explicit Equation(const std::vector<uint64_t>& data)
    {
        result = data[0];
        inputs = data | std::views::drop(1) | std::ranges::to<std::vector>();
    }
};

struct Data
{
    std::vector<Equation> equations;
};

auto parse_input(const std::string& input) -> Data
{
    namespace bp = boost::parser;

    constexpr auto parser = *(bp::ulong_ >> ": " >> (bp::ulong_ % ' ') >> bp::eol);
    const auto result = bp::parse(input, parser);

    // I expected the `ulong >> ': ' >> (ulong & ' ')` to parse to tuple<ulong, vec<ulong>>. weird.
    // const std::optional<std::vector<std::tuple<uint64_t, std::vector<uint64_t>>>> &res = result;
    const std::optional<std::vector<std::vector<uint64_t>>>& res = result;

    if (!res.has_value())
    {
        throw std::runtime_error("Failed to parse input");
    }

    const auto equations = res.value() | std::ranges::to<std::vector<Equation>>();
    return Data{equations};
}

/// This concept limits the `F` to only be something that can be called as a function with a signature
/// `(uint64_t, uint64_t) -> uint64_t`

template <typename F>
concept BinaryOperation = requires(const F& f)
{
    requires std::is_invocable_v<F, uint64_t, uint64_t>;
    requires std::is_same_v<std::invoke_result_t<F, uint64_t, uint64_t>, uint64_t>;
};

template <typename... F> requires ((sizeof...(F) > 0) && (BinaryOperation<F> && ...))
auto is_valid_inner(
    const uint64_t target,
    const std::span<const uint64_t> remaining_inputs,
    const uint64_t current_total,
    const F&... f
) -> bool
{
    if (current_total > target)
    {
        return false;
    }

    if (remaining_inputs.empty())
    {
        return current_total == target;
    }

    const uint64_t next = remaining_inputs[0];
    const std::span<const uint64_t> remaining_inputs_rest = remaining_inputs | std::views::drop(1);

    return
        (is_valid_inner(target, remaining_inputs_rest, f(current_total, next), f...) || ...);
}


template <typename... F> requires ((sizeof...(F) > 0) && (BinaryOperation<F> && ...))
auto is_valid(const Equation& equation, F&... f) -> bool
{
    return is_valid_inner(equation.result, equation.inputs, 0, f...);
}

auto add(const uint64_t a, const uint64_t b) -> uint64_t
{
    return a + b;
}

auto mul(const uint64_t a, const uint64_t b) -> uint64_t
{
    return a * b;
}

uint64_t Day07::part1() const
{
    const auto data = parse_input(this->input);

    uint64_t total = 0;

    for (const Equation& eq : data.equations)
    {
        if (is_valid(eq, mul, add))
        {
            total += eq.result;
        }
    }

    return total;
}

auto concat(const uint64_t a, const uint64_t b) -> uint64_t
{
    const auto digit_count = static_cast<uint64_t>(std::floor(std::log10(b)));
    const auto shift = static_cast<uint64_t>(std::pow(10, digit_count + 1));
    return a * shift + b;
}

uint64_t Day07::part2() const
{
    const auto data = parse_input(this->input);

    uint64_t total = 0;

    for (const Equation& eq : data.equations)
    {
        if (is_valid(eq, concat, mul, add))
        {
            total += eq.result;
        }
    }

    return total;
}
