#include "day06.2.h"

#include <algorithm>
#include <tuple>
#include <vector>
#include <ranges>
#include <set>
#include <unordered_set>

typedef std::tuple<int, int> position_t;
template <>
struct std::hash<position_t>
{
    std::size_t operator()(position_t const &p) const noexcept
    {
        std::size_t h1 = std::hash<int>{}(std::get<0>(p));
        std::size_t h2 = std::hash<int>{}(std::get<1>(p));
        return h1 ^ (h2 << 1);
    }
};

namespace
{

    enum class map_tile_t : int
    {
        Empty = 0,
        Obstacle = 1,
        Guard = 2,
    };
    enum class direction_t : int
    {
        Up = 0,
        Right = 1,
        Down = 2,
        Left = 3,
    };
    struct input_t
    {
        std::vector<std::vector<map_tile_t>> obstacles;
        int row_count;
        int column_count;
        position_t guard_start;
    };
    input_t parse_input(const std::string_view &input)
    {
        int column_count = 0;
        const auto result = input 
            | std::views::split('\n')
            | std::views::filter([](const auto &line) { return !line.empty(); })
            | std::views::transform([&column_count](const auto &line)
            {
                column_count = line.size();
                return line
                    | std::views::transform([](const char c) {
                        switch (c) {
                            case '#':
                                return map_tile_t::Obstacle;
                            case '^':
                                return map_tile_t::Guard;
                            default:
                                return map_tile_t::Empty;
                        }
                }); })
                | std::ranges::to<std::vector<std::vector<map_tile_t>>>();

        int row_count = result.size();

        position_t guard_start{-1, -1};
        for (const auto &[row_index, row] : result | std::views::enumerate)
        {
            for (const auto &[col_index, tile] : row | std::views::enumerate)
            {
                if (tile == map_tile_t::Guard)
                {
                    guard_start = {col_index, row_index};
                }
            }
        }
        if (guard_start == position_t{-1, -1})
        {
            throw std::runtime_error("No guard start position found");
        }

        return {result, row_count, column_count, guard_start};
    }

    position_t forward_position(const position_t &current, const direction_t &dir)
    {
        auto [x, y] = current;
        switch (dir)
        {
        case direction_t::Up:
            return {x, y - 1};
        case direction_t::Down:
            return {x, y + 1};
        case direction_t::Left:
            return {x - 1, y};
        case direction_t::Right:
            return {x + 1, y};
        default:
            throw std::runtime_error("Unknown direction");
        }
    }

    bool in_bounds(const position_t &pos, const input_t &input)
    {
        auto [x, y] = pos;
        return x >= 0 && x < input.column_count && y >= 0 && y < input.row_count;
    }

    direction_t turn_right(const direction_t &dir)
    {
        return static_cast<direction_t>((static_cast<int>(dir) + 1) % 4);
    }

    std::tuple<position_t, direction_t> do_step(const position_t &current_pos, const direction_t &current_dir, const input_t &input)
    {
        position_t forward = forward_position(current_pos, current_dir);

        auto [fx, fy] = forward;
        if (!in_bounds(forward, input) || input.obstacles[fy][fx] != map_tile_t::Obstacle)
        {
            return {forward, current_dir};
        }

        direction_t new_dir = turn_right(current_dir);
        return {current_pos, new_dir};
    }
}

std::size_t Day06_2::part1() const
{
    const input_t input = parse_input(this->input);

    std::unordered_set<position_t> visited_positions{};
    position_t current_position = input.guard_start;
    direction_t direction = direction_t::Up;

    while (in_bounds(current_position, input))
    {
        visited_positions.insert(current_position);
        auto [new_position, new_direction] = do_step(current_position, direction, input);
        current_position = new_position;
        direction = new_direction;
    }

    return visited_positions.size();
}

bool does_loop(const input_t &input)
{
    // TODO
    return false;
}

int Day06_2::part2() const
{
    auto input = parse_input(this->input);

    int total = 0;

    for (int x = 0; x < input.column_count; ++x) {
        for (int y = 0; y < input.row_count; ++y) {
            if (input.obstacles[y][x] != map_tile_t::Empty) {
                continue;
            }

            input.obstacles[y][x] = map_tile_t::Obstacle;
            if (does_loop(input)) {
                total++;
            }
            input.obstacles[y][x] = map_tile_t::Empty;
        }
    }

    return total;
}
