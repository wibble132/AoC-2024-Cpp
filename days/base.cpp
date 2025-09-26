#include "base.h"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

// template<typename T1, typename T2>
// BaseDay<T1, T2>::~BaseDay() = default;

std::string get_filename(const int n)
{
    // Read input from file "inputs/day{n:02}.txt"
    std::stringstream ss;
    ss << "inputs/day" << std::setw(2) << std::setfill('0') << n << ".txt";
    return ss.str();
}

template<typename T1, typename T2>
BaseDay<T1, T2>::BaseDay(const int n) : day_number(n)
{
    std::string filename = get_filename(n);

    std::ifstream input_file(filename);
    if (!input_file.is_open())
    {
        throw std::runtime_error("Failed to open input file: " + filename);
    }

    uintmax_t size = std::filesystem::file_size(filename);
    input = std::string(size, '\0');
    input_file.read(&input[0], static_cast<std::streamsize>(size));
}

template<typename T1, typename T2>
void BaseDay<T1, T2>::run()
{
    const auto start = std::chrono::high_resolution_clock::now();
    const T1 p1 = part1();
    const auto part1_end = std::chrono::high_resolution_clock::now();
    const T2 p2 = part2();
    const auto part2_end = std::chrono::high_resolution_clock::now();

    const auto part1_time = std::chrono::duration<double, std::micro>(part1_end - start);
    const auto part2_time = std::chrono::duration<double, std::micro>(part2_end - part1_end);

    std::cout << "Day " << this->day_number << '\n';
    std::cout << "  Part 1: " << p1 << " in " << part1_time.count() << "us\n";
    std::cout << "  Part 2: " << p2 << " in " << part2_time.count() << "us\n";
    std::cout << std::endl;
}
