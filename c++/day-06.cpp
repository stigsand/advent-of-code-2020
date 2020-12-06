// https://adventofcode.com/2020/day/6

#include <fmt/os.h>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <numeric>
#include <regex>
#include <set>
#include <string_view>
#include <vector>

using individual_answer = std::set<char>;
using group_answer = std::vector<individual_answer>;

template <typename CountStrategy>
auto sum_group_answers(std::vector<group_answer> const & answers, CountStrategy strategy)
{
    return std::accumulate(answers.begin(), answers.end(), std::size_t { 0 },
            [&] (auto s, auto const & answer) { return s + strategy(answer); });
}

std::size_t any_answered_count(group_answer const & answer)
{
    return std::accumulate(answer.begin(), answer.end(), std::set<char> {},
        [] (auto const & curr, auto const & ind_answer) {
            std::set<char> new_curr;
            std::set_union(curr.begin(), curr.end(), ind_answer.begin(), ind_answer.end(), std::inserter(new_curr, new_curr.end()));
            return new_curr;
        }).size();
}

std::size_t all_answered_count(group_answer const & answer)
{
    assert(!answer.empty());
    return std::accumulate(answer.begin() + 1, answer.end(), answer[0],
        [] (auto const & curr, auto const & ind_answer) {
            std::set<char> new_curr;
            std::set_intersection(curr.begin(), curr.end(), ind_answer.begin(), ind_answer.end(), std::inserter(new_curr, new_curr.end()));
            return new_curr;
        }).size();
}

template <typename It>
std::vector<group_answer> read_group_answers(It begin, It end)
{
    std::vector<group_answer> answers;
    std::regex group_answer_regex { R"(\n\n)" };
    std::regex ind_answer_regex { R"(\n)" };
    for (auto group_it = std::regex_token_iterator<It> { begin, end, group_answer_regex, -1 }; group_it != std::regex_token_iterator<It> {}; ++group_it) {
        answers.emplace_back();
        for (auto ind_it = std::regex_token_iterator<It> { group_it->first, group_it->second, ind_answer_regex, -1 }; ind_it != std::regex_token_iterator<It> {}; ++ind_it) {
            answers.back().emplace_back();
            for (auto it = ind_it->first; it != ind_it->second; ++it)
                answers.back().back().insert(*it);
        }
    }
    return answers;
}

void test()
{
    const std::string_view input =
            "abc\n"
            "\n"
            "a\n"
            "b\n"
            "c\n"
            "\n"
            "ab\n"
            "ac\n"
            "\n"
            "a\n"
            "a\n"
            "a\n"
            "a\n"
            "\n"
            "b\n";
    auto answers = read_group_answers(input.begin(), input.end());
    assert(sum_group_answers(answers, any_answered_count) == 11);
    assert(sum_group_answers(answers, all_answered_count) == 6);
}

int main()
{
    test();

    std::ifstream in("input/day-06");
    std::vector<char> input(std::istreambuf_iterator<char> { in }, std::istreambuf_iterator<char> {});
    auto answers = read_group_answers(input.begin(), input.end());
    fmt::print("Sum of answer count (any): {}\n", sum_group_answers(answers, any_answered_count));
    fmt::print("Sum of answer count (all): {}\n", sum_group_answers(answers, all_answered_count));
}
