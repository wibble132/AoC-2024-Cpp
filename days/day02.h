#ifndef AOC_2024_DAY02_H
#define AOC_2024_DAY02_H

#include "base.h"

class Day02 final : public BaseDay
{
public:
    explicit Day02() : BaseDay(2)
    {
    }

    [[nodiscard]] size_t part1() const;
    [[nodiscard]] size_t part2() const;
};


#endif //AOC_2024_DAY02_H
