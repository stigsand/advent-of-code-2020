// https://adventofcode.com/2020/day/1

#include <fmt/os.h>
#include <gsl/span>
#include <array>
#include <cassert>
#include <fstream>
#include <iterator>
#include <optional>
#include <tuple>
#include <unordered_set>
#include <vector>

std::optional<std::tuple<int, int>> find_addend_pair(gsl::span<const int> numbers, int sum)
{
    std::unordered_set<int> seen;
    for (auto a: numbers) {
        auto i = sum - a;
        if (seen.count(i))
            return std::tuple { i, a };
        else
            seen.insert(a);
    }
    return std::nullopt;
}

std::optional<std::tuple<int, int, int>> find_addend_triple(gsl::span<const int> numbers, int sum)
{
    assert(numbers.size() >= 3);
    for (std::size_t i = 0; i < numbers.size() - 2; ++i)
        if (auto p = find_addend_pair(numbers.subspan(i + 1), sum - numbers[i]); p)
            return std::tuple { numbers[i], std::get<0>(*p), std::get<1>(*p) };
    return std::nullopt;
}

void test()
{
    std::array const numbers { 1721, 979, 366, 299, 675, 1456 };

    auto p = find_addend_pair(numbers, 2020);
    assert(std::get<0>(p.value()) * std::get<1>(p.value()) == 514579);

    auto t = find_addend_triple(numbers, 2020);
    assert(std::get<0>(t.value()) * std::get<1>(t.value()) * std::get<2>(t.value()) == 241861950);
}

int main()
{
    test();

    std::ifstream in("input/day-01");
    std::vector<int> const expenses {
            std::istream_iterator<int>(in),
	        std::istream_iterator<int>() };

    auto p = find_addend_pair(expenses, 2020);
    fmt::print("Product of pair: {}\n", std::get<0>(p.value()) * std::get<1>(p.value()));

    auto t = find_addend_triple(expenses, 2020);
    fmt::print("Product of triple: {}\n", std::get<0>(t.value()) * std::get<1>(t.value()) * std::get<2>(t.value()));
}
