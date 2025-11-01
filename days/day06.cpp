#include "day06.h"

#include <algorithm>
#include <tuple>
#include <vector>
#include <ranges>
#include <set>
#include <unordered_set>

typedef std::tuple<int, int> position_t;
enum class direction_t : uint8_t
{
    Up,
    Right,
    Down,
    Left,
};
struct input_t
{
    std::set<position_t> obstacles;
    int rows;
    int cols;
    position_t guard_start;
};

input_t parse_input(const std::string_view &input)
{
    int rows = 0, cols = 0;
    position_t guard_position{-1, -1};
    auto result = input | std::views::split('\n') | std::views::enumerate | std::views::transform([&cols, &rows, &guard_position](const auto &x)
                                                                                                  {
            const auto &[line_index_ref, line] = x;
            std::size_t line_index = line_index_ref;
            rows = line_index;
            return line
                | std::views::enumerate
                | std::views::filter([line_index, &cols, &guard_position](const auto& t)
                {
                    const auto& [col_index, c] = t;

                    cols = col_index;
                    if (c == '^') guard_position = position_t{col_index, line_index};

                    return c == '#';
                })
                | std::views::transform([line_index](const auto& y)
                {
                    return position_t{std::get<0>(y), line_index};
                }); }) |
                  std::views::join | std::ranges::to<std::set<position_t>>();

    return {result, rows + 1, cols + 1, guard_position};
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
    return x >= 0 && x < input.rows && y >= 0 && y < input.cols;
}

direction_t turn_right(const direction_t &dir)
{
    return static_cast<direction_t>((static_cast<uint8_t>(dir) + 1) % 4);
}

std::tuple<position_t, direction_t> do_step(const position_t &current_pos, const direction_t &current_dir, const input_t &input)
{
    position_t forward = forward_position(current_pos, current_dir);

    if (!input.obstacles.contains(forward))
    {
        return {forward, current_dir};
    }

    // Forward is blocked, turn to the right
    direction_t new_dir = turn_right(current_dir);
    return {current_pos, new_dir};
}

std::size_t Day06::part1() const
{
    const auto input = parse_input(this->input);

    std::set<position_t> visited_positions{};
    position_t current_pos = input.guard_start;
    direction_t current_dir = direction_t::Up;

    visited_positions.insert(current_pos);

    while (true)
    {
        auto [next_pos, next_dir] = do_step(current_pos, current_dir, input);
        if (!in_bounds(next_pos, input))
        {
            break;
        }

        current_pos = next_pos;
        current_dir = next_dir;

        visited_positions.insert(next_pos);
    }

    return visited_positions.size();
}

bool does_loop(const input_t &input)
{
    position_t current_pos = input.guard_start;
    direction_t current_dir = direction_t::Up;

    std::set<std::tuple<position_t, direction_t>> visited_states{};
    while (true)
    {
        visited_states.insert({current_pos, current_dir});

        auto next = do_step(current_pos, current_dir, input);
        if (visited_states.contains(next))
        {
            // Repeated a state, must be a loop
            return true;
        }

        auto [next_pos, next_dir] = next;
        if (!in_bounds(next_pos, input))
        {
            // Left the area, no loop
            return false;
        }

        current_pos = next_pos;
        current_dir = next_dir;
    }
}

int part2_old(const std::string &in)
{
    auto input = parse_input(in);

    int total = 0;

    for (int x = 0; x < input.cols; ++x)
    {
        for (int y = 0; y < input.rows; ++y)
        {
            auto new_position = std::make_tuple(x, y);
            if (input.obstacles.contains(new_position) || new_position == input.guard_start)
            {
                continue;
            }

            input.obstacles.insert(new_position);
            if (does_loop(input))
            {
                total++;
            }
            input.obstacles.erase(new_position);
        }
    }

    return total;
}

void do_step_to_wall(position_t &current_pos, direction_t &current_dir, const input_t &input)
{
    position_t forward = forward_position(current_pos, current_dir);
    while (!input.obstacles.contains(forward) && in_bounds(current_pos, input)) 
    {
        current_pos = forward;
        forward = forward_position(current_pos, current_dir);
    }

    current_dir = turn_right(current_dir);
}

bool does_loop_2(const input_t &input, std::set<std::tuple<position_t, direction_t>> visited_states, position_t current_position, direction_t current_direction) {
    while (true) {
        if (visited_states.contains({current_position, current_direction})) {
            return true;
        }

        visited_states.insert({current_position, current_direction});
        do_step_to_wall(current_position, current_direction, input);
        if (!in_bounds(current_position, input)) {
            return false;
        }
    }

    return false;
}

int Day06::part2() const
{
    auto input = parse_input(this->input);

    position_t current_position = input.guard_start;
    direction_t current_direction = direction_t::Up;

    // Only includes the states just after turning, to reduce the size
    std::set<std::tuple<position_t, direction_t>> visited_states{};

    std::set<position_t> attempted_new_obstacles{};

    // Disallow placing a new obstacle at the starting position, if we somehow returned to it
    attempted_new_obstacles.insert(current_position);

    int total = 0;

    while (in_bounds(current_position, input))
    {
        position_t forward = forward_position(current_position, current_direction);

        if (input.obstacles.contains(forward))
        {
            current_direction = turn_right(current_direction);
            visited_states.insert({current_position, current_direction});
            continue;
        }

        // Have we already tried this position as a new obstacle? Move forward
        if (attempted_new_obstacles.contains(forward)) {
            current_position = forward;
            continue;
        }

        // See what happens if there was an obstacle placed at `forward`
        input.obstacles.insert(forward);
        // This does copy `visited_states`, but we want to preserve the current set and modify it
        if (does_loop_2(input, visited_states, current_position, turn_right(current_direction))) {
            total++;
        }
        input.obstacles.erase(forward);

        attempted_new_obstacles.insert(forward);
        current_position = forward;
    }

    return total;
}
