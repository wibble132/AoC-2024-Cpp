#ifndef AOC_2024_DAY02_H
#define AOC_2024_DAY02_H

#include "base.h"

class Day02 final : public BaseDay<size_t, size_t>
{
public:
    explicit Day02() : BaseDay(2)
    {
    }

    size_t part1() override;
    size_t part2() override;
};


#endif //AOC_2024_DAY02_H
