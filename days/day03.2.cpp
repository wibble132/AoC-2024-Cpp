#include "day03.2.h"
#include <cstring>

// An attempt at making a more optimised solution
// On my puzzle input, running 1 million repeats took an average of
//  Part1: 3.88872us
//  Part2: 2.24342us
// Taking 16x the puzzle input, running 1 million repeats took an average of
//  Part1: 58.6677us
//  Part2: 57.0584us


int Day03_2::part1() const
{
    const char* p = input.data();
    const char* const end = p + input.size();
    int total = 0;

    while (true)
    {
        const void* mpos = std::memchr(p, 'm', static_cast<size_t>(end - p));
        if (!mpos) break;
        p = static_cast<const char*>(mpos);
        if (p + 8 >= end) break;

        if (p[1] != 'u' || p[2] != 'l' || p[3] != '(')
        {
            ++p;
            continue;
        }

        p += 4;

        auto d = static_cast<unsigned char>(*p - '0');
        if (d > 9) continue;

        int a = d;
        ++p;
        d = static_cast<unsigned char>(*p - '0');
        if (d <= 9)
        {
            a = a * 10 + d;
            ++p;
            d = static_cast<unsigned char>(*p - '0');
            if (d <= 9)
            {
                a = a * 10 + d;
                ++p;
            }
        }

        if (*p != ',') continue;
        ++p;

        d = static_cast<unsigned char>(*p - '0');
        if (d > 9) continue;

        int b = d;
        ++p;
        d = static_cast<unsigned char>(*p - '0');
        if (d <= 9)
        {
            b = b * 10 + d;
            ++p;
            d = static_cast<unsigned char>(*p - '0');
            if (d <= 9)
            {
                b = b * 10 + d;
                ++p;
            }
        }


        if (*p == ')')
        {
            ++p;
            total += a * b;
        }
    }

    return total;
}

int Day03_2::part2() const
{
    const char* p = input.data();
    const char* const end = p + input.size();
    int total = 0;
    bool enabled = true;

    const void* m_pos = std::memchr(p, 'm', static_cast<size_t>(end - p));
    const void* d_pos = std::memchr(p, 'd', static_cast<size_t>(end - p));
    while (true)
    {
        if (!m_pos) break; // No more mul instructions - give up
        if (!enabled && !d_pos) break; // Disabled and no more do instructions - give up

        if (!enabled || (d_pos && d_pos < m_pos))
        {
            p = static_cast<const char*>(d_pos);
            if (end - p < 7)
            {
                break;
            }

            if (p[1] == 'o')
            {
                if (!enabled && p[2] == '(' && p[3] == ')')
                {
                    // do()
                    enabled = true;
                    p += 4;
                }
                else if (enabled && p[2] == 'n' && p[3] == '\'' && p[4] == 't' && p[5] == '(' && p[6] == ')')
                {
                    // don't()
                    enabled = false;
                    p += 7;
                }
                else
                    p += 2;
            }
            else
                p += 1;

            // Have we skipped an 'm' while not enabled?
            if (m_pos < p) m_pos = std::memchr(p, 'm', static_cast<size_t>(end - p));
            d_pos = std::memchr(p, 'd', static_cast<size_t>(end - p));
            continue;
        }

        p = static_cast<const char*>(m_pos);
        if (p + 8 >= end) break;

        if (p[1] != 'u' || p[2] != 'l' || p[3] != '(')
        {
            ++p;
            m_pos = std::memchr(p, 'm', static_cast<size_t>(end - p));
            continue;
        }

        p += 4;
        m_pos = std::memchr(p, 'm', static_cast<size_t>(end - p));

        // -- The following is the same as for part 1 --

        auto d = static_cast<unsigned char>(*p - '0');
        if (d > 9) continue;

        int a = d;
        ++p;
        d = static_cast<unsigned char>(*p - '0');
        if (d <= 9)
        {
            a = a * 10 + d;
            ++p;
            d = static_cast<unsigned char>(*p - '0');
            if (d <= 9)
            {
                a = a * 10 + d;
                ++p;
            }
        }

        if (*p != ',') continue;
        ++p;

        d = static_cast<unsigned char>(*p - '0');
        if (d > 9) continue;

        int b = d;
        ++p;
        d = static_cast<unsigned char>(*p - '0');
        if (d <= 9)
        {
            b = b * 10 + d;
            ++p;
            d = static_cast<unsigned char>(*p - '0');
            if (d <= 9)
            {
                b = b * 10 + d;
                ++p;
            }
        }


        if (*p == ')')
        {
            ++p;
            total += a * b;
        }
    }

    return total;
}
