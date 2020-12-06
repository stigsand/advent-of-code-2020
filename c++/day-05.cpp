// https://adventofcode.com/2020/day/5

#include <fmt/os.h>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <numeric>
#include <regex>
#include <string>
#include <string_view>
#include <vector>

struct seat {
    unsigned int row;
    unsigned int col;
};

unsigned int id(seat s)
{
    assert(s.row <= 127 && s.col <= 7);
    return s.row * 8 + s.col;
}

seat decode_seat(std::string_view encoding)
{
    assert(std::regex_match(encoding.begin(), encoding.end(), std::regex { R"((F|B){7}(L|R){3})" }));
    struct range {
        unsigned int start;
        unsigned int size;
    };
    auto make_partitioner = [] (char lower, char upper)
    {
        return [=] (range r, char part) {
            return part == lower
                    ? range { r.start, r.size / 2 }
                    : range { r.start + r.size / 2, r.size / 2 };
        };
    };
    auto row_range = std::accumulate(encoding.begin(), encoding.begin() + 7, range { 0, 128 }, make_partitioner('F', 'B'));
    auto col_range = std::accumulate(encoding.begin() + 7, encoding.end(), range { 0, 8 }, make_partitioner('L', 'R'));
    assert(row_range.size == 1 && col_range.size == 1);
    return { row_range.start, col_range.start };
}

void test()
{
    assert(id(decode_seat("FBFBBFFRLR")) == 357);
    assert(id(decode_seat("BFFFBBFRRR")) == 567);
    assert(id(decode_seat("FFFBBBFRRR")) == 119);
    assert(id(decode_seat("BBFFBBFRLL")) == 820);
}

int main()
{
    test();

    std::ifstream in("input/day-05");
    std::vector<unsigned int> seat_ids;
    std::string seat_encoding;
    while (std::getline(in, seat_encoding))
        seat_ids.push_back(id(decode_seat(seat_encoding)));

    assert(!seat_ids.empty());
    fmt::print("Highest seat ID: {}\n", *std::max_element(seat_ids.begin(), seat_ids.end()));

    std::sort(seat_ids.begin(), seat_ids.end());
    auto before_missing_it = std::adjacent_find(seat_ids.begin(), seat_ids.end(), [] (auto s1, auto s2) {
        return s2 - s1 == 2;
    });
    assert(before_missing_it != seat_ids.end());
    fmt::print("Missing seat ID: {}\n", *before_missing_it + 1);
}
