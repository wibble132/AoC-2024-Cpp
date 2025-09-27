#ifndef AOC_2024_DAY01_H
#define AOC_2024_DAY01_H

#include "base.h"

class Day01 final : public BaseDay
{
public:
    explicit Day01() : BaseDay(1)
    {
    }

    [[nodiscard]] int part1() const;
    [[nodiscard]] int part2() const;
};


#endif //AOC_2024_DAY01_H
