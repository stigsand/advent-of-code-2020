// https://adventofcode.com/2020/day/4

#include <fmt/os.h>
#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <functional>
#include <iterator>
#include <regex>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

using passport = std::unordered_map<std::string, std::string>;

bool is_loosely_valid(passport const & p)
{
    static const std::array<std::string_view, 7> required_fields { "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid" };
    return std::all_of(required_fields.begin(), required_fields.end(), [&] (auto field) { return p.count(std::string(field)) > 0; });
}

template <typename T, typename U>
bool between_inclusive(T t, U min, U max)
{
    return t >= min && t <= max;
}

bool byr_valid(std::string const & byr)
{
    return std::regex_match(byr, std::regex { R"(\d{4})" }) && between_inclusive(std::stoul(byr), 1920, 2002);
}

bool iyr_valid(std::string const & iyr)
{
    return std::regex_match(iyr, std::regex { R"(\d{4})" }) && between_inclusive(std::stoul(iyr), 2010, 2020);
}

bool eyr_valid(std::string const & eyr)
{
    return std::regex_match(eyr, std::regex { R"(\d{4})" }) && between_inclusive(std::stoul(eyr), 2020, 2030);
}

bool hgt_valid(std::string const & hgt)
{
    std::smatch match;
    return std::regex_match(hgt, match, std::regex { R"((\d{2,3})cm|(\d{2,3})in)" })
            && ( (match[1].matched && between_inclusive(std::stoul(match[1]), 150, 193))
                    || (match[2].matched && between_inclusive(std::stoul(match[2]), 59, 76)) );
}

bool hcl_valid(std::string const & hcl)
{
    return std::regex_match(hcl, std::regex { R"(#[0-9a-f]{6})" });
}

bool ecl_valid(std::string const & ecl)
{
    return std::regex_match(ecl, std::regex { R"(amb|blu|brn|gry|grn|hzl|oth)" });
}

bool pid_valid(std::string const & pid)
{
    return std::regex_match(pid, std::regex { R"(\d{9})" });
}

bool is_strictly_valid(passport const & p)
{
    static const std::vector<std::pair<std::string, std::function<bool (std::string const &)>>> field_validators {
            { "byr", byr_valid }, { "iyr", iyr_valid }, { "eyr", eyr_valid },
            { "hgt", hgt_valid }, { "hcl", hcl_valid }, { "ecl", ecl_valid },
            { "pid", pid_valid } };
    return std::all_of(field_validators.begin(), field_validators.end(), [&] (auto const & fv_pair) {
        auto field_it = p.find(fv_pair.first);
        return field_it != p.end() && fv_pair.second(field_it->second);
    });
}

template <typename Validator>
auto count_valid(std::vector<passport> const & passports, Validator validator)
{
    return std::count_if(passports.begin(),passports.end(), validator);
}

template <typename It>
std::vector<passport> read_passports(It begin, It end)
{
    std::vector<passport> passports;
    std::regex passport_regex { R"(\n\n)" };
    std::regex kv_regex { R"(([^:\s]+):(\S*))" };
    for (auto it = std::regex_token_iterator<It> { begin, end, passport_regex, -1 }; it != std::regex_token_iterator<It> {}; ++it) {
        passports.emplace_back();
        for (auto kv_it = std::regex_iterator<It> { it->first, it->second, kv_regex }; kv_it != std::regex_iterator<It> {}; ++kv_it)
            passports.back().emplace((*kv_it)[1], (*kv_it)[2]);
    }
    return passports;
}

void test()
{
    const std::string_view input =
            "ecl:gry pid:860033327 eyr:2020 hcl:#fffffd\n"
            "byr:1937 iyr:2017 cid:147 hgt:183cm\n"
            "\n"
            "iyr:2013 ecl:amb cid:350 eyr:2023 pid:028048884\n"
            "hcl:#cfa07d byr:1929\n"
            "\n"
            "hcl:#ae17e1 iyr:2013\n"
            "eyr:2024\n"
            "ecl:brn pid:760753108 byr:1931\n"
            "hgt:179cm\n"
            "\n"
            "hcl:#cfa07d eyr:2025 pid:166559648\n"
            "iyr:2011 ecl:brn hgt:59in\n";
    auto passports = read_passports(input.begin(), input.end());
    assert(count_valid(passports, is_loosely_valid) == 2);

    assert(byr_valid("2002"));
    assert(!byr_valid("2003"));
    assert(hgt_valid("60in"));
    assert(hgt_valid("190cm"));
    assert(!hgt_valid("190in"));
    assert(!hgt_valid("190"));
    assert(hcl_valid("#123abc"));
    assert(!hcl_valid("#123abz"));
    assert(!hcl_valid("123abc"));
    assert(ecl_valid("brn"));
    assert(!ecl_valid("wat"));
    assert(pid_valid("000000001"));
    assert(!pid_valid("0123456789"));

    const std::string_view all_invalid_input =
            "eyr:1972 cid:100\n"
            "hcl:#18171d ecl:amb hgt:170 pid:186cm iyr:2018 byr:1926\n"
            "\n"
            "iyr:2019\n"
            "hcl:#602927 eyr:1967 hgt:170cm\n"
            "ecl:grn pid:012533040 byr:1946\n"
            "\n"
            "hcl:dab227 iyr:2012\n"
            "ecl:brn hgt:182cm pid:021572410 eyr:2020 byr:1992 cid:277\n"
            "\n"
            "hgt:59cm ecl:zzz\n"
            "eyr:2038 hcl:74454a iyr:2023\n"
            "pid:3556412378 byr:2007\n";
    auto invalid_passports = read_passports(all_invalid_input.begin(), all_invalid_input.end());
    assert(std::none_of(invalid_passports.begin(), invalid_passports.end(), is_strictly_valid));

    const std::string_view all_valid_input =
            "pid:087499704 hgt:74in ecl:grn iyr:2012 eyr:2030 byr:1980\n"
            "hcl:#623a2f\n"
            "\n"
            "eyr:2029 ecl:blu cid:129 byr:1989\n"
            "iyr:2014 pid:896056539 hcl:#a97842 hgt:165cm\n"
            "\n"
            "hcl:#888785\n"
            "hgt:164cm byr:2001 iyr:2015 cid:88\n"
            "pid:545766238 ecl:hzl\n"
            "eyr:2022\n"
            "\n"
            "iyr:2010 hgt:158cm hcl:#b6652a ecl:blu byr:1944 eyr:2021 pid:093154719\n";
    auto valid_passports = read_passports(all_valid_input.begin(), all_valid_input.end());
    assert(std::all_of(valid_passports.begin(), valid_passports.end(), is_strictly_valid));
}

int main()
{
    test();

    std::ifstream in("input/day-04");
    std::vector<char> input(std::istreambuf_iterator<char> { in }, std::istreambuf_iterator<char> {});
    auto passports = read_passports(input.begin(), input.end());
    fmt::print("Valid passports (loosely): {}\n", count_valid(passports, is_loosely_valid));
    fmt::print("Valid passports (strictly): {}\n", count_valid(passports, is_strictly_valid));
}
