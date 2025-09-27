#ifndef AOC_2024_BASE_H
#define AOC_2024_BASE_H

#include <string>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <algorithm>
#include <sstream>
#include <stdexcept>

class BaseDay
{
public:
    explicit BaseDay(int n);
    virtual ~BaseDay() = default;

    int day_number;
    std::string input;
};

inline BaseDay::BaseDay(const int n) : day_number(n)
{
    std::ostringstream ss;
    ss << std::setw(2) << std::setfill('0') << n;
    const std::filesystem::path filename = std::filesystem::path("inputs") / ("day" + ss.str() + ".txt");

    std::ifstream f(filename, std::ios::binary);
    if (!f) throw std::runtime_error("Failed to open input file: " + filename.string() + ". Please provide the input data.");
    input.assign(std::istreambuf_iterator(f), std::istreambuf_iterator<char>());
}

constexpr int iterations = 1000;

template <typename T> requires requires(const T& t)
{
    { t.part1() };
    { t.part2() };
    requires std::is_integral_v<decltype(t.part1())>;
    requires std::is_integral_v<decltype(t.part2())>;
}
auto run_many(const T& day) -> std::tuple<
    std::vector<decltype(day.part1())>,
    std::vector<decltype(day.part2())>,
    std::vector<double>,
    std::vector<double>
>
{
    using hrc = std::chrono::high_resolution_clock;
    using std::chrono::duration;

    std::vector<double> p1_times(iterations);
    std::vector<double> p2_times(iterations);

    std::vector<decltype(day.part1())> p1_results(iterations);
    std::vector<decltype(day.part2())> p2_results(iterations);

    for (int i = 0; i < iterations; i++)
    {
        const auto start = hrc::now();
        p1_results[i] = day.part1();
        const auto end = hrc::now();
        p1_times[i] = duration<double, std::micro>(end - start).count();
    }
    for (int i = 0; i < iterations; i++)
    {
        const auto start = hrc::now();
        p2_results[i] = day.part2();
        const auto end = hrc::now();
        p2_times[i] = duration<double, std::micro>(end - start).count();
    }
    return std::make_tuple(p1_results, p2_results, p1_times, p2_times);
}

template <typename T>
concept RunnableDay = requires(const T& t)
{
    requires std::is_integral_v<decltype(t.part1())>;
    requires std::is_integral_v<decltype(t.part2())>;
    requires std::is_integral_v<decltype(t.day_number)>;
};

template <typename T> requires RunnableDay<T>
void run(const T& day)
{
    auto [p1_results, p2_results, p1_times, p2_times] = run_many(day);

    const auto p1_mean = std::accumulate(p1_times.begin(), p1_times.end(), 0.0) / iterations;
    const auto p2_mean = std::accumulate(p2_times.begin(), p2_times.end(), 0.0) / iterations;

    std::cout << "Day " << day.day_number << '\n';
    std::cout << "  Part 1: " << p1_results[0] << " in " << p1_mean << "us\n";
    if (std::adjacent_find(p1_results.begin(), p1_results.end(), std::not_equal_to<>{}) != p1_results.end())
        std::cout << "    Err: Inconsistent results!";

    std::cout << "  Part 2: " << p2_results[0] << " in " << p2_mean << "us\n";
    if (std::adjacent_find(p2_results.begin(), p2_results.end(), std::not_equal_to<>{}) != p2_results.end())
        std::cout << "    Err: Inconsistent results!";

    std::cout << std::endl;
}

#endif //AOC_2024_BASE_H
