#ifndef AOC_2024_BASE_H
#define AOC_2024_BASE_H

#include <string>

template<typename T1, typename T2>
class BaseDay
{
public:
    explicit BaseDay(int n);
    virtual ~BaseDay() = default;
    void run();

protected:
    virtual T1 part1() = 0;
    virtual T2 part2() = 0;

    int day_number;
    std::string input;
};


#endif //AOC_2024_BASE_H