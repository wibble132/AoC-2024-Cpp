#ifndef AOC_2024_DAY03_1_H
#define AOC_2024_DAY03_1_H

#include "base.h"

class Day03_1 final : public BaseDay
{
public:
    explicit Day03_1() : BaseDay(3)
    {
    }

    [[nodiscard]] unsigned int part1() const;
    [[nodiscard]] unsigned int part2() const;
};


#endif //AOC_2024_DAY03_1_H
