#include "../common/util.h"
#include "../common/textbox.h"

struct Solver
{
    std::vector<std::string> combos;
    std::vector<std::string> designs;

    Solver() {}

    Solver(const std::vector<std::string>& lines) {
        for(auto& t : tokenize(lines.front(), ", ")) {
            combos.push_back(t);
        }

        std::for_each(lines.begin() + 2, lines.end(), [&](const std::string& s) {
                designs.push_back(s);
            });
    }

    bool solve(const std::string_view& desired) {
        if (desired.empty())
            return true;

        std::vector<int> matches;

        for (auto& partial : combos)
            if (desired.starts_with(partial))
                if (solve(desired.substr(partial.size())))
                    return true;

        return false;
    }

    uint64_t solve_all() {
        return std::count_if(designs.begin(), designs.end(), [&](const std::string& s) {
            return solve(s);
        });
    }
};


void selftest() {
    assert(Solver({
        "r, wr, b, g, bwu, rb, gb, br",
        "",
        "brwrr",
        "bggr",
        "gbbr",
        "rrbgbr",
        "ubwu",
        "bwurrg",
        "brgr",
        "bbrgwb"
    }).solve_all() == 6);
}

int main() {
    selftest();

    auto lines = read_file_lines("input.txt");
    if (lines.empty()) {
        std::cout << "unable to load input" << std::endl;
        return 0;
    }

    auto solver = Solver(lines);

    int64_t answer1 = solver.solve_all();
    std::cout << "Answer Part 1: " << answer1 << std::endl;
}
