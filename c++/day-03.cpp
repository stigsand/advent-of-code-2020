// https://adventofcode.com/2020/day/3

#include <fmt/os.h>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <numeric>
#include <string_view>
#include <utility>
#include <vector>

struct square { std::size_t x; std::size_t y; };

struct tree_map {
public:
    template <typename It>
    tree_map(It begin, It end)
    {
        std::vector<bool> row;
        for (auto it = begin; it != end; ++it) {
            if (*it == '\n') {
                rows.push_back(std::move(row));
                row.clear();
            }
            else
                row.push_back(*it == '#');
        }
        auto not_equal_lengths = [] (auto const & lhs, auto const & rhs) { return lhs.size() != rhs.size(); };
        assert(std::adjacent_find(rows.begin(), rows.end(), not_equal_lengths) == rows.end());
        assert(!rows.empty() && !rows[0].empty());
    }

    bool has_tree(square pos) const
    {
        return rows[pos.y][pos.x % rows[0].size()];
    }

    std::size_t n_rows() const { return rows.size(); }

private:
    std::vector<std::vector<bool>> rows;
};

square follow_slope(square pos, square slope)
{
    return { pos.x + slope.x, pos.y + slope.y };
}

auto count_trees(tree_map const & map, square start_pos, square slope)
{
    std::size_t n = 0;
    for (auto pos = start_pos; pos.y < map.n_rows(); pos = follow_slope(pos, slope))
        if (map.has_tree(pos))
            ++n;
    return n;
}

auto tree_count_product(tree_map const & map, square start_pos, std::vector<square> const & slopes)
{
    return std::accumulate(slopes.begin(), slopes.end(), std::size_t { 1 }, [&] (auto p, auto slope) {
        return p * count_trees(map, start_pos, slope);
    });
}

void test()
{
    const std::string_view input =
            "..##.......\n"
            "#...#...#..\n"
            ".#....#..#.\n"
            "..#.#...#.#\n"
            ".#...##..#.\n"
            "..#.##.....\n"
            ".#.#.#....#\n"
            ".#........#\n"
            "#.##...#...\n"
            "#...##....#\n"
            ".#..#...#.#\n";
    auto map = tree_map(input.begin(), input.end());
    assert(count_trees(map, { 0, 0 }, { 3, 1 }) == 7);
    assert(tree_count_product(map, { 0, 0 }, { { 1, 1 }, { 3, 1 }, { 5, 1 }, { 7, 1 }, { 1, 2 } }) == 336);
}

int main()
{
    test();

    std::ifstream in("input/day-03");
    auto map = tree_map(std::istreambuf_iterator<char> { in }, std::istreambuf_iterator<char> {});
    fmt::print("Trees encountered: {}\n", count_trees(map, { 0, 0 }, { 3, 1 }));
    fmt::print("Trees encountered product: {}\n", tree_count_product(map, { 0, 0 }, { { 1, 1 }, { 3, 1 }, { 5, 1 }, { 7, 1 }, { 1, 2 } }));
}
