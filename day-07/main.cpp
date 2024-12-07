#include "../common/util.h"

struct Equation {
    long long result;
    std::vector<int> values;
    bool valid = false;

    bool test(long long acc = 0, int i = 0) {
        // none left
        if (i >= values.size())
            return acc == result;
        
        return test(acc + values[i], i+1) ||
               test(acc * values[i], i+1);
    }
};

int main() {
    long long answer1 = 0;
    //long long answer2 = 0;

    auto lines = read_file_lines("input.txt");
    if (lines.empty()) {
        std::cout << "unable to load input" << std::endl;
        return 0;
    }

    auto equations = std::vector<Equation>();

    for (auto& line : lines) {
        auto s = tokenize(line, ": ");
        
        auto result = std::stoll(s.front());
        auto nums   = parse_ints(s.back(), " "); 

        equations.push_back({result, nums});
    }

    for (auto& eq : equations) {
        answer1 += eq.test()? eq.result : 0;
    } 

    std::cout << "Answer Part 1: " << answer1 << std::endl;
    //std::cout << "Answer Part 2: " << answer2 << std::endl;
}