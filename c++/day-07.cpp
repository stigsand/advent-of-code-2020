// https://adventofcode.com/2020/day/7

#include <fmt/os.h>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <numeric>
#include <regex>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

using content = std::unordered_map<std::string, std::size_t>;
using bag_rules = std::unordered_map<std::string, content>;

bool search_bag_colors_containing(
        std::string const & target_color, std::string const & start_color,
        bag_rules const & rules, std::unordered_map<std::string, bool> & processed_colors)
{
    if (auto it = processed_colors.find(start_color); it != processed_colors.end())
        return it->second;
    auto found = false;
    for (auto & [color, _]: rules.at(start_color))
        if (color == target_color || search_bag_colors_containing(target_color, color, rules, processed_colors)) {
            found = true;
            break;
        }
    processed_colors.emplace(start_color, found);
    return found;
}

auto find_bag_colors_containing(std::string const & target_color, bag_rules const & rules)
{
    std::unordered_map<std::string, bool> processed_colors;
    for (auto const & [color, _]: rules)
        if (color != target_color)
            search_bag_colors_containing(target_color, color, rules, processed_colors);
    return std::count_if(processed_colors.begin(), processed_colors.end(), [] (auto const & kv) { return kv.second; });
}

auto bags_inside(std::string const & color, bag_rules const & rules)
{
    auto & content = rules.at(color);
    return std::accumulate(content.begin(), content.end(), std::size_t { 0 },
            [&] (auto count, auto const & kv) { return count + kv.second * (1 + bags_inside(kv.first, rules)); });
}

template <typename It>
bag_rules read_bag_rules(It begin, It end)
{
    bag_rules rules;
    std::regex rule_separator { R"(\.\n)" };
    std::regex rule_regex { R"((.*) bags contain (?:no other bags|(.*)))" };
    std::regex content_item_separator { R"(, )" };
    std::regex content_item_regex { R"((\d+) (.*) bags?)" };
    for (auto rule_it = std::regex_token_iterator<It> { begin, end, rule_separator, -1 }; rule_it != std::regex_token_iterator<It> {}; ++rule_it) {
        std::match_results<It> rule_match;
        std::regex_match(rule_it->first, rule_it->second, rule_match, rule_regex);
        assert(!rule_match.empty());
        auto [new_rule, inserted] = rules.emplace(rule_match.str(1), content {});
        assert(inserted);
        if (rule_match[2].matched)
            for (auto content_item_it = std::regex_token_iterator<It> { rule_match[2].first, rule_match[2].second, content_item_separator, -1 };
                    content_item_it != std::regex_token_iterator<It> {}; ++content_item_it) {
                std::match_results<It> content_item_match;
                std::regex_match(content_item_it->first, content_item_it->second, content_item_match, content_item_regex);
                assert(!content_item_match.empty());
                auto [new_content_item, inserted] = new_rule->second.emplace(content_item_match.str(2), std::stoul(content_item_match.str(1)));
                assert(inserted && new_content_item->second > 0);
            }
    }
    return rules;
}

void test()
{
    const std::string_view input =
            "light red bags contain 1 bright white bag, 2 muted yellow bags.\n"
            "dark orange bags contain 3 bright white bags, 4 muted yellow bags.\n"
            "bright white bags contain 1 shiny gold bag.\n"
            "muted yellow bags contain 2 shiny gold bags, 9 faded blue bags.\n"
            "shiny gold bags contain 1 dark olive bag, 2 vibrant plum bags.\n"
            "dark olive bags contain 3 faded blue bags, 4 dotted black bags.\n"
            "vibrant plum bags contain 5 faded blue bags, 6 dotted black bags.\n"
            "faded blue bags contain no other bags.\n"
            "dotted black bags contain no other bags.\n";
    auto rules = read_bag_rules(input.begin(), input.end());
    assert(find_bag_colors_containing("shiny gold", rules) == 4);
    assert(bags_inside("shiny gold", rules) == 32);

    const std::string_view input2 =
            "shiny gold bags contain 2 dark red bags.\n"
            "dark red bags contain 2 dark orange bags.\n"
            "dark orange bags contain 2 dark yellow bags.\n"
            "dark yellow bags contain 2 dark green bags.\n"
            "dark green bags contain 2 dark blue bags.\n"
            "dark blue bags contain 2 dark violet bags.\n"
            "dark violet bags contain no other bags.\n";
    auto rules2 = read_bag_rules(input2.begin(), input2.end());
    assert(bags_inside("shiny gold", rules2) == 126);
}

int main()
{
    test();

    std::ifstream in("input/day-07");
    std::vector<char> input(std::istreambuf_iterator<char> { in }, std::istreambuf_iterator<char> {});
    auto rules = read_bag_rules(input.begin(), input.end());
    fmt::print("Bag colors containing shiny gold bag: {}\n", find_bag_colors_containing("shiny gold", rules));
    fmt::print("Bags inside shiny gold bag: {}\n", bags_inside("shiny gold", rules));
}
