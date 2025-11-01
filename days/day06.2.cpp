#include "day06.2.h"

#include <tuple>
#include <vector>
#include <ranges>
#include <flat_set>
#include <cstdint>

namespace
{
    typedef struct position
    {
        uint8_t x;
        uint8_t y;

        bool operator==(const position& other) const
        {
            return this->x == other.x && this->y == other.y;
        }

        std::strong_ordering operator<=>(const position& other) const
        {
            // Use lexicographical ordering to allow being placed in a set
            if (const auto x_compare = this->x <=> other.x; x_compare != std::strong_ordering::equal)
            {
                return x_compare;
            }

            return this->y <=> other.y;
        }
    } position_t;

    enum class map_tile_t : uint8_t
    {
        Empty = 0,
        Obstacle = 1,
        Guard = 2,
    };

    enum class direction_t : uint8_t
    {
        Up = 0,
        Right = 1,
        Down = 2,
        Left = 3,
    };

    struct obstacles_t
    {
        explicit obstacles_t(const std::vector<std::vector<map_tile_t>>& tiles)
        {
            this->row_count = static_cast<uint8_t>(tiles.size());
            this->column_count = static_cast<uint8_t>(tiles[0].size());
            this->tiles = tiles | std::views::join | std::ranges::to<std::vector<map_tile_t>>();
        }

        map_tile_t& operator[](const position_t& pos)
        {
            return tiles[pos.y * column_count + pos.x];
        }

        map_tile_t operator[](const position_t& pos) const noexcept
        {
            if (pos.x >= column_count || pos.y >= row_count)
                return map_tile_t::Empty;

            return tiles[pos.y * column_count + pos.x];
        }

        [[nodiscard]] auto enumerate() const noexcept
        {
            return this->tiles | std::views::chunk(this->column_count) | std::views::enumerate | std::views::transform(
                    [](const auto&& x)
                    {
                        const auto& [row_index,row] = x;
                        return row | std::views::enumerate | std::views::transform([row_index](const auto&& y)
                        {
                            const auto& [column_index, tile] = y;
                            const auto position = position_t{
                                static_cast<uint8_t>(column_index),
                                static_cast<uint8_t>(row_index)
                            };
                            return std::tuple<position_t, map_tile_t>{position, tile};
                        });
                    })
                | std::views::join;
        }

    private:
        std::vector<map_tile_t> tiles;
        uint8_t row_count;
        uint8_t column_count;
    };

    struct input_t
    {
        obstacles_t obstacles;
        uint8_t row_count{};
        uint8_t column_count{};
        position_t guard_start{};
    };

    map_tile_t char_to_tile(const char c)
    {
        switch (c)
        {
        case '#':
            return map_tile_t::Obstacle;
        case '^':
            return map_tile_t::Guard;
        default:
            return map_tile_t::Empty;
        }
    }

    input_t parse_input(const std::string_view& input)
    {
        const auto tiles = input
            | std::views::split('\n')
            | std::views::filter([](const auto& line) { return !line.empty(); })
            | std::views::transform([](const auto& line)
            {
                return line | std::views::transform(char_to_tile);
            })
            | std::ranges::to<std::vector<std::vector<map_tile_t>>>();

        const auto column_count = static_cast<uint8_t>(tiles[0].size());
        const auto row_count = static_cast<uint8_t>(tiles.size());

        position_t guard_start{std::numeric_limits<uint8_t>::max(), std::numeric_limits<uint8_t>::max()};
        for (const auto& [row_index, row] : tiles | std::views::enumerate)
        {
            for (const auto& [col_index, tile] : row | std::views::enumerate)
            {
                if (tile == map_tile_t::Guard)
                {
                    guard_start = {static_cast<uint8_t>(col_index), static_cast<uint8_t>(row_index)};
                }
            }
        }

        if (guard_start == position_t{std::numeric_limits<uint8_t>::max(), std::numeric_limits<uint8_t>::max()})
        {
            throw std::runtime_error("No guard start position found");
        }

        return {obstacles_t{tiles}, row_count, column_count, guard_start};
    }

    position_t forward_position(const position_t& current, const direction_t& dir)
    {
        auto [x, y] = current;

        switch (dir)
        {
        case direction_t::Up:
            y -= 1;
            break;
        case direction_t::Down:
            y += 1;
            break;
        case direction_t::Left:
            x -= 1;
            break;
        case direction_t::Right:
            x += 1;
            break;
        }

        return {x, y};
    }

    bool in_bounds(const position_t& pos, const input_t& input)
    {
        auto [x, y] = pos;
        return x < input.column_count && y < input.row_count;
    }

    direction_t turn_right(const direction_t& dir)
    {
        return static_cast<direction_t>((static_cast<uint8_t>(dir) + 1) % 4);
    }

    std::tuple<position_t, direction_t> do_step(const position_t& current_pos, const direction_t& current_dir,
                                                const input_t& input)
    {
        const position_t forward = forward_position(current_pos, current_dir);
        if (!in_bounds(forward, input) || input.obstacles[forward] != map_tile_t::Obstacle)
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

    std::flat_set<position_t> visited_positions{};
    auto current_position = input.guard_start;
    auto direction = direction_t::Up;

    while (in_bounds(current_position, input))
    {
        visited_positions.insert(current_position);
        auto [new_position, new_direction] = do_step(current_position, direction, input);
        current_position = new_position;
        direction = new_direction;
    }

    return visited_positions.size();
}

void do_step_to_wall(position_t& current_pos, direction_t& current_dir, const input_t& input)
{
    position_t forward = forward_position(current_pos, current_dir);
    while (in_bounds(current_pos, input) && input.obstacles[forward] != map_tile_t::Obstacle)
    {
        current_pos = forward;
        forward = forward_position(current_pos, current_dir);
    }

    current_dir = turn_right(current_dir);
}

bool does_loop(const input_t& input, std::flat_set<std::tuple<position_t, direction_t>> visited_states)
{
    visited_states.clear();
    auto current_position = input.guard_start;
    auto current_direction = direction_t::Up;

    while (true)
    {
        if (visited_states.contains({current_position, current_direction}))
        {
            return true;
        }

        visited_states.insert({current_position, current_direction});
        do_step_to_wall(current_position, current_direction, input);
        if (!in_bounds(current_position, input))
        {
            return false;
        }
    }
}

int Day06_2::part2() const
{
    auto input = parse_input(this->input);

    int total = 0;

    // Reuse for every step to avoid repeated allocations
    const std::flat_set<std::tuple<position_t, direction_t>> visited_states{};

    for (const auto&& [pos, tile] : input.obstacles.enumerate())
    {
        // Can only place tiles in empty positions
        if (tile != map_tile_t::Empty)
        {
            continue;
        }

        input.obstacles[pos] = map_tile_t::Obstacle;
        if (does_loop(input, visited_states))
            total++;
        input.obstacles[pos] = map_tile_t::Empty;
    }

    return total;
}
