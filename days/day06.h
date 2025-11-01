#ifndef AOC_2024_DAY06_H
#define AOC_2024_DAY06_H
#include "base.h"


class Day06 final : public BaseDay
{
public:
    explicit Day06() : BaseDay(6)
    {
    }

    [[nodiscard]] std::size_t part1() const;
    [[nodiscard]] int part2() const;
};


#endif //AOC_2024_DAY06_H