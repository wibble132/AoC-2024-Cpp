#ifndef AOC_2024_DAY05_H
#define AOC_2024_DAY05_H
#include "base.h"


class Day05 final : public BaseDay
{
public:
    explicit Day05() : BaseDay(5)
    {
    }

    [[nodiscard]] int part1() const;
    [[nodiscard]] int part2() const;
};


#endif //AOC_2024_DAY05_H