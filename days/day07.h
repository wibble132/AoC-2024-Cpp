#ifndef AOC_2024_DAY07_H
#define AOC_2024_DAY07_H
#include "base.h"


class Day07 final : public BaseDay
{
public:
    explicit Day07() : BaseDay(7)
    {
    }

    [[nodiscard]] uint64_t part1() const;
    [[nodiscard]] uint64_t part2() const;
};


#endif //AOC_2024_DAY07_H