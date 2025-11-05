#include "day07.h"

#include <vector>
#include <boost/parser/parser.hpp>

struct Equation
{
    uint64_t result;
    std::vector<uint64_t> inputs;

    explicit Equation(const std::span<const uint64_t> data)
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

    // Each line is a number, then `: `, then several numbers separated by spaces.
    constexpr auto parser = *(bp::ulong_ >> ": " >> (bp::ulong_ % ' ') >> bp::eol);
    const auto result = bp::parse(input, parser);

    // I expected the `ulong >> ': ' >> (ulong & ' ')` to parse to tuple<ulong, vec<ulong>>. weird.
    // const std::optional<std::vector<std::tuple<uint64_t, std::vector<uint64_t>>>> &res = result;

    // Instead, the inner loop all just gets shoved into a single std::vector.
    // Oh well, that's manageable still, I created a constructor for `Equation` that deals with splitting them out.
    const std::optional<std::vector<std::vector<uint64_t>>>& res = result;

    if (!res.has_value())
    {
        throw std::runtime_error("Failed to parse input");
    }

    // `ranges::to` is really useful!
    // Here, it takes `res.value()` as a range of `std::vector<uint64_t>`,
    // then turns each one into an `Equation` using the constructor,
    // then packages that into a std::vector
    const auto equations = res.value() | std::ranges::to<std::vector<Equation>>();
    return Data{equations};
}

template <typename F>
concept BinaryOperation = requires(const F& f, const uint64_t a, const uint64_t b)
{
    { f(a, b) } -> std::same_as<uint64_t>;
};


// This uses a "Parameter pack" - the "..."
// This is just a template that gets repeated.
// E.g., a simple example could be
// template<typename ...T> auto add(T&... a) -> int
// {
//     return (a + ...);
// }
// and then I can have `add(1,2,4)` or `add(4,6,5,3,27)`.
// For each different set of arguments, a completely new function will get generated and compiled!
//
// Here, I am using it to allow this function to take any number of operations - this is useful since part 2 adds
// an extra one compared to part 1.
//
// Also, the `BinaryOperation` concept can be used instead of `typename` where I am showing which types can be input

template <BinaryOperation... F>
auto is_valid_inner(
    const uint64_t target,
    const std::span<const uint64_t> remaining_inputs, // std::span doesn't own or copy the elements, unlike std::vector
    const uint64_t current_total,
    const F&... f
) -> bool
{
    // Optimisation - since all the operators are increasing, we can give up after overshooting the target
    if (current_total > target) return false;

    // Run out of inputs - we have finished the calculation! Check if we have the correct result
    if (remaining_inputs.empty()) return current_total == target;

    // [next, ...tail] = remaining_inputs; - split off the next element, so we can recurse
    const uint64_t next = remaining_inputs[0];
    const std::span<const uint64_t> tail = remaining_inputs.subspan(1);

    // Double parameter pack expansion!
    // The `f...` is just passing all the operators to the inner function - nice and simple
    // The `f(current_total,next)` and the `|| ...` is also expanded, this
    //   makes the recursive call for each `f` and uses `||` to combine the results
    // e.g. `f1, f2` would expand as `is_valid_inner(<>, f1(<>), f1, f2) || is_valid_inner(<>, f2(<>), f1, f2)`
    return (is_valid_inner(target, tail, f(current_total, next), f...) || ...);
}


template <BinaryOperation... F>
auto is_valid(const Equation& equation, F&... f) -> bool
{
    return is_valid_inner(equation.result, equation.inputs, 0, f...);
}

template <BinaryOperation... F>
auto do_part(const std::span<const Equation> equations, const F&... f)
{
    return std::ranges::fold_left(
        equations
        | std::views::filter([&f...](const Equation& eq)-> bool { return is_valid(eq, f...); })
        | std::views::transform([](const Equation& eq) -> uint64_t { return eq.result; }),
        0,
        std::plus{}
    );
}

constexpr auto add = std::plus<uint64_t>{};
constexpr auto mul = std::multiplies<uint64_t>{};


uint64_t Day07::part1() const
{
    const auto [equations] = parse_input(this->input);
    return do_part(equations, mul, add);
}

auto concat(const uint64_t a, const uint64_t b) -> uint64_t
{
    const auto digit_count = static_cast<uint64_t>(std::ceil(std::log10(b)));
    const auto shift = static_cast<uint64_t>(std::pow(10, digit_count));
    return a * shift + b;
}

uint64_t Day07::part2() const
{
    const auto [equations] = parse_input(this->input);
    return do_part(equations, concat, mul, add);
}
