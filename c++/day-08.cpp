// https://adventofcode.com/2020/day/8

#include <fmt/os.h>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <optional>
#include <regex>
#include <set>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

struct acc {};
void execute(acc, int arg, int & accumulator, std::size_t & next_instr)
{
    accumulator += arg;
    ++next_instr;
}

struct jmp {};
void execute(jmp, int arg, int & accumulator, std::size_t & next_instr)
{
    assert(arg >= 0 || -arg <= next_instr);
    next_instr += arg;
}

struct nop {};
void execute(nop, int arg, int & accumulator, std::size_t & next_instr)
{
    ++next_instr;
}

using operation = std::variant<acc, jmp, nop>;

struct instruction {
    operation op;
    int arg;
};

struct game_console {
    int accumulator = 0;
    std::size_t next_instr = 0;
    std::set<int> instructions_executed;
};

void execute_instruction(game_console & m, instruction instr)
{
    m.instructions_executed.insert(m.next_instr);
    std::visit([&] (auto op) { execute(op, instr.arg, m.accumulator, m.next_instr); }, instr.op);
}

void run_until_loop_detection(game_console & m, std::vector<instruction> const & instructions)
{
    while (m.instructions_executed.count(m.next_instr) == 0)
        execute_instruction(m, instructions.at(m.next_instr));
}

bool run(game_console & m, std::vector<instruction> const & instructions)
{
    while (m.instructions_executed.count(m.next_instr) == 0) {
        if (m.next_instr == instructions.size())
            return true;
        execute_instruction(m, instructions.at(m.next_instr));
    }
    return false;
}

std::optional<int> accumulator_on_termination(std::vector<instruction> const & instructions)
{
    for (std::size_t i = 0; i < instructions.size(); ++i) {
        auto & op = instructions[i].op;
        if (std::holds_alternative<acc>(op))
            continue;
        auto mutated_instructions = instructions;
        mutated_instructions[i].op = std::holds_alternative<nop>(op) ? operation { jmp {} } : nop {};
        game_console m;
        if (run(m, mutated_instructions))
            return m.accumulator;
    }
    return std::nullopt;
}

template <typename It>
std::vector<instruction> read_instructions(It begin, It end)
{
    std::vector<instruction> instructions;
    std::regex instr_separator { R"(\n)" };
    std::regex instr_regex { R"((acc|jmp|nop) ([+|-]\d+))" };
    for (auto instr_it = std::regex_token_iterator<It> { begin, end, instr_separator, -1 }; instr_it != std::regex_token_iterator<It> {}; ++instr_it) {
        std::match_results<It> instr_match;
        std::regex_match(instr_it->first, instr_it->second, instr_match, instr_regex);
        assert(!instr_match.empty());
        auto op = [&] () -> operation {
            if (instr_match[1] == "acc") return acc {};
            else if (instr_match[1] == "jmp") return jmp {};
            else return nop {};
        }();
        instructions.push_back({ op, std::stoi(instr_match[2]) });
    }
    return instructions;
}

void test()
{
    const std::string_view input =
            "nop +0\n"
            "acc +1\n"
            "jmp +4\n"
            "acc +3\n"
            "jmp -3\n"
            "acc -99\n"
            "acc +1\n"
            "jmp -4\n"
            "acc +6\n";
    auto instructions = read_instructions(input.begin(), input.end());

    game_console m;
    run_until_loop_detection(m, instructions);
    assert(m.accumulator == 5);

    assert(accumulator_on_termination(instructions) == 8);
}

int main()
{
    test();

    std::ifstream in("input/day-08");
    std::vector<char> input(std::istreambuf_iterator<char> { in }, std::istreambuf_iterator<char> {});
    auto instructions = read_instructions(input.begin(), input.end());

    game_console m;
    run_until_loop_detection(m, instructions);
    fmt::print("Accumulator on loop detection: {}\n", m.accumulator);

    fmt::print("Accumulator on normal termination: {}\n", accumulator_on_termination(instructions).value());
}
