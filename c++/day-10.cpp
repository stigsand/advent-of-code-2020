// https://adventofcode.com/2020/day/10

#include <fmt/os.h>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <numeric>
#include <unordered_map>
#include <utility>
#include <vector>

template <typename It>
std::vector<int> find_jolt_diffs(It begin, It end)
{
    std::vector<int> v { begin, end };
    std::sort(v.begin(), v.end());
    std::adjacent_difference(v.begin(), v.end(), v.begin());
    v.push_back(3);
    return v;
}

using counts = std::pair<std::size_t, std::size_t>;

counts count_1_and_3_jolt_diffs(std::vector<int> const & diffs)
{
    return { std::count(diffs.begin(), diffs.end(), 1) , std::count(diffs.begin(), diffs.end(), 3) };
}

std::size_t count_arrangements(std::vector<int> const & diffs, std::size_t i, std::unordered_map<std::size_t, std::size_t> & results)
{
    if (i >= diffs.size())
        return 1;
    if (auto it = results.find(i); it != results.end())
        return it->second;
    std::size_t cnt = 0;
    int diff = 0;
    for (auto j = i; j < diffs.size(); ++j) {
        diff += diffs[j];
        if (diff > 3)
            break;
        cnt += count_arrangements(diffs, j + 1, results);
    }
    results.emplace(i, cnt);
    return cnt;
}

std::size_t count_arrangements(std::vector<int> const & diffs)
{
    std::unordered_map<std::size_t, std::size_t> results;
    return count_arrangements(diffs, 0, results);
}

void test()
{
    int const adapter_ratings[] = { 16, 10, 15, 5, 1, 11, 7, 19, 6, 12, 4 };
    auto jolt_diffs = find_jolt_diffs(std::begin(adapter_ratings), std::end(adapter_ratings));
    assert(count_1_and_3_jolt_diffs(jolt_diffs) == counts(7, 5));
    assert(count_arrangements(jolt_diffs) == 8);

    int const adapter_ratings2[] = {
            28, 33, 18, 42, 31, 14, 46, 20, 48, 47, 24, 23, 49, 45, 19,
            38, 39, 11, 1, 32, 25, 35, 8, 17, 7, 9, 4, 2, 34, 10, 3 };
    auto jolt_diffs2 = find_jolt_diffs(std::begin(adapter_ratings2), std::end(adapter_ratings2));
    assert(count_1_and_3_jolt_diffs(jolt_diffs2) == counts(22, 10));
    assert(count_arrangements(jolt_diffs2) == 19208);
}

int main()
{
    test();

    std::ifstream in("input/day-10");
    auto jolt_diffs = find_jolt_diffs(std::istream_iterator<int> { in }, std::istream_iterator<int> {});
    auto diff_counts = count_1_and_3_jolt_diffs(jolt_diffs);
    fmt::print("1-jolt differences * 3-jolt differences: {}\n", diff_counts.first * diff_counts.second);
    fmt::print("Distinct arrangements: {}\n", count_arrangements(jolt_diffs));
}
