#ifndef AOC_2024_DAY03_H
#define AOC_2024_DAY03_H

#include "base.h"

class Day03 final : public BaseDay<int, int>
{
public:
    explicit Day03() : BaseDay(3)
    {
    }

protected:
    int part1() override;
    int part2() override;
};


#endif //AOC_2024_DAY03_H
