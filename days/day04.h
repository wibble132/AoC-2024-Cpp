#ifndef AOC_2024_DAY04_H
#define AOC_2024_DAY04_H
#include "base.h"


class Day04 final : public BaseDay
{
public:
    explicit Day04() : BaseDay(4)
    {
    }

    [[nodiscard]] int part1() const;
    [[nodiscard]] int part2() const;
};


#endif //AOC_2024_DAY04_H