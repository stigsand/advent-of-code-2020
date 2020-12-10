// https://adventofcode.com/2020/day/9

#include <fmt/os.h>
#include <gsl/span>
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using int_t = std::uint64_t;

std::optional<std::pair<int_t, int_t>> find_addend_pair(gsl::span<const int_t> numbers, int_t sum, std::unordered_set<int_t> & observed)
{
    for (auto a: numbers) {
        auto i = sum - a;
        if (observed.count(i) && i != a)
            return std::pair { i, a };
        else
            observed.insert(a);
    }
    return std::nullopt;
}

std::optional<int_t> find_invalid_number(gsl::span<const int_t> numbers, std::size_t preamble_size)
{
    std::unordered_set<int_t> seen_in_preamble;
    assert(numbers.size() > preamble_size);
    for (std::size_t i = preamble_size; i < numbers.size(); ++i) {
        if (!find_addend_pair(numbers.subspan(i - preamble_size, preamble_size), numbers[i], seen_in_preamble).has_value())
            return numbers[i];
        else
            seen_in_preamble.erase(numbers[i - preamble_size]);
    }
    return std::nullopt;
}

std::optional<gsl::span<const int_t>> find_sub_array(gsl::span<const int_t> numbers, int_t sum)
{
    std::unordered_map<int_t, std::size_t> prefix_sums;
    int_t curr_sum = 0;
    for (std::size_t i = 0; i < numbers.size(); i++) {
        curr_sum += numbers[i];
        if (curr_sum == sum)
            return numbers.first(i);
        else if (auto it = prefix_sums.find(curr_sum - sum); it != prefix_sums.end())
            return numbers.subspan(it->second + 1, i - it->second);
        else
            prefix_sums.emplace(curr_sum, i);
    }
    return std::nullopt;
}

int_t smallest_largest_sum(gsl::span<const int_t> numbers)
{
    assert(!numbers.empty());
    return *std::min_element(numbers.begin(), numbers.end()) + *std::max_element(numbers.begin(), numbers.end());
}

void test()
{
    int_t const numbers[] = { 35, 20, 15, 25, 47, 40, 62, 55, 65, 95, 102, 117, 150, 182, 127, 219, 299, 277, 309, 576 };
    auto invalid_number = find_invalid_number(numbers, 5).value();
    assert(invalid_number == 127);
    assert(smallest_largest_sum(find_sub_array(numbers, invalid_number).value()) == 62);
}

int main()
{
    test();

    std::ifstream in("input/day-09");
    std::vector<int_t> input(std::istream_iterator<int_t> { in }, std::istream_iterator<int_t> {});
    auto invalid_number = find_invalid_number(input, 25).value();
    fmt::print("Invalid number: {}\n", invalid_number);
    fmt::print("Encryption weakness: {}\n", smallest_largest_sum(find_sub_array(input, invalid_number).value()));
}
