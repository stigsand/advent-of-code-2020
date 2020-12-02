// https://adventofcode.com/2020/day/2

#include <fmt/os.h>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <iterator>
#include <regex>
#include <string>
#include <string_view>
#include <vector>

struct pw_policy {
    int a;
    int b;
    char c;

    struct occurence_rule {};
    struct position_rule {};
};

struct pw_entry {
    pw_policy policy;
    std::string pw;
};

bool check(pw_entry const & e, pw_policy::occurence_rule)
{
    auto count = std::count(e.pw.begin(), e.pw.end(), e.policy.c);
    return count >= e.policy.a && count <= e.policy.b;
}

bool check(pw_entry const & e, pw_policy::position_rule)
{
    auto match_at = [&] (auto pos) {
        return pos >= 1 && pos <= e.pw.size() && e.pw[pos - 1] == e.policy.c;
    };
    return match_at(e.policy.a) != match_at(e.policy.b);  // boolean xor
}

template <typename Rule>
auto count_valid(std::vector<pw_entry> const & entries, Rule)
{
    return std::count_if(entries.begin(), entries.end(), [] (auto const & e) { return check(e, Rule {}); });
}

template <typename It>
std::vector<pw_entry> read_pw_entries(It begin, It end)
{
    std::vector<pw_entry> e;
    std::regex entry_regex { R"((\d+)-(\d+) (.): (\S*))" };
    for (auto it = std::regex_iterator<It> { begin, end, entry_regex }; it != std::regex_iterator<It> {}; ++it) {
        auto const & match = *it;
        e.push_back({ { std::stoi(match[1]), std::stoi(match[2]), match.str(3)[0] }, match[4] });
    }
    return e;
}

void test()
{
    constexpr std::string_view input = R"(
            1-3 a: abcde
            1-3 b: cdefg
            2-9 c: ccccccccc)";
    auto pw_entries = read_pw_entries(input.begin(), input.end());
    assert(count_valid(pw_entries, pw_policy::occurence_rule {}) == 2);
    assert(count_valid(pw_entries, pw_policy::position_rule {}) == 1);
}

int main()
{
    test();

    std::ifstream in("input/day-02");
    std::vector<char> input(std::istreambuf_iterator<char> { in }, std::istreambuf_iterator<char> {});
    auto pw_entries = read_pw_entries(input.begin(), input.end());
    fmt::print("Valid passwords (occurrence policy) : {}\n",
            count_valid(pw_entries, pw_policy::occurence_rule {}));
    fmt::print("Valid passwords (position policy): {}\n",
            count_valid(pw_entries, pw_policy::position_rule {}));
}
