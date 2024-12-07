#include "../common/util.h"

bool is_safe_dec(const std::vector<int>& levels) {
    for (int i = 1; i < levels.size(); i++) {
        int d = levels[i-1] - levels[i];
            if (d < 1 || d > 3)
                return false;
    }
    return true;
}

bool is_safe_inc(const std::vector<int>& levels) {
    for (int i = 1; i < levels.size(); i++) {
        int d = levels[i] - levels[i-1];
            if (d < 1 || d > 3)
                return false;
    }
    return true;
}


bool is_safe(const std::vector<int>& report) {
    return is_safe_dec(report)
        || is_safe_inc(report);
}

bool is_safe_enough(const std::vector<int>& report) {
    if (is_safe(report))
        return true;

    for (int i = 0; i < report.size(); i++) {
        auto modded = report;
        modded.erase(modded.begin() + i);

        if (is_safe(modded))
            return true;
    }
    return false;
}

int main() {
    int answer1 = 0;
    int answer2 = 0;

    auto lines = read_file_lines("input.txt");
    auto reports = std::vector<std::vector<int>>();

    for (auto &line : lines)
        reports.emplace_back(parse_ints(line, " "));

    for (auto& report : reports) {
        answer1 += is_safe(report);
        answer2 += is_safe_enough(report);
    }

    std::cout << "Answer Part 1: " << answer1 << std::endl;
    std::cout << "Answer Part 2: " << answer2 << std::endl;


}





