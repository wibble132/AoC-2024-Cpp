#ifndef AOC_2024_DAY06_2_H
#define AOC_2024_DAY06_2_H
#include "base.h"


class Day06_2 final : public BaseDay
{
public:
    explicit Day06_2() : BaseDay(6)
    {
    }

    [[nodiscard]] std::size_t part1() const;
    [[nodiscard]] int part2() const;
};


#endif //AOC_2024_DAY06_H