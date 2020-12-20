// https://adventofcode.com/2020/day/11

#include <fmt/os.h>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iterator>
// #include <numeric>
// #include <unordered_map>
#include <utility>
#include <vector>

struct seat_layout {
public:
    template <typename It>
    seat_layout(It begin, It end)
    {
        std::vector<char> row;
        for (auto it = begin; it != end; ++it) {
            auto c = *it;
            if (c == '\n') {
                rows.push_back(std::move(row));
                row.clear();
            }
            else {
                assert(c == '#' || c == '.' || c == 'L');
                row.push_back(c);
            }
        }
        auto not_equal_lengths = [] (auto const & lhs, auto const & rhs) { return lhs.size() != rhs.size(); };
        assert(std::adjacent_find(rows.begin(), rows.end(), not_equal_lengths) == rows.end());
        assert(!rows.empty() && !rows[0].empty());
    }

    seat_layout apply_rules() const
    {
        auto next = *this;
        //for 
        return next;
    }

    bool operator==(seat_layout const & other) const
    {
        return rows == other.rows;
    }

    bool operator!=(seat_layout const & other) const
    {
        return !(rows == other.rows);
    }

    auto count_occupied() const
    {
        return std::size_t { 0 };
    }

private:
    std::size_t adjacent_occupied(size_t x, size_t y) const
    {
        std::size_t cnt = 0;
        std::pair x_range = { x > 0 ? x - 1 : 0, x < rows[0].size() - 1 ? x + 1 : rows[0].size() - 1 };
        std::pair y_range = { y > 0 ? y - 1 : 0, y < rows.size() - 1 ? y + 1 : rows.size() - 1 };
        for (std::size_t i = 0; i < rows[0].size(); ++i)
            for (std::size_t j = 0; j < rows.size(); ++j)
                if (rows[j][i] == '#' && !(x == i && y == j))
                    ++cnt;
        return cnt;
    }

    std::vector<std::vector<char>> rows;
};

seat_layout apply_rules_until_stable(seat_layout const & layout)
{
    //seat_layout stable;
    return layout;
    /*do {
        auto next_layout = 
    } while (stable)*/
}

void test()
{
    const std::string_view input =
            "L.LL.LL.LL\n"
            "LLLLLLL.LL\n"
            "L.L.L..L..\n"
            "LLLL.LL.LL\n"
            "L.LL.LL.LL\n"
            "L.LLLLL.LL\n"
            "..L.L.....\n"
            "LLLLLLLLLL\n"
            "L.LLLLLL.L\n"
            "L.LLLLL.LL\n";
    auto layout = seat_layout { input.begin(), input.end() };
    assert(apply_rules_until_stable(layout).count_occupied() == 37 + 1);
}

int main()
{
    test();

    std::ifstream in("input/day-11");
    auto layout = seat_layout { std::istreambuf_iterator<char> { in }, std::istreambuf_iterator<char> {} };
    fmt::print("Occupied seats in stable layout: {}\n", apply_rules_until_stable(layout).count_occupied());
}
