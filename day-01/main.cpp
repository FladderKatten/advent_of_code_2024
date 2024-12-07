#include "../common/util.h"

int main() {
    int answer1 = 0;
    int answer2 = 0;
    auto lines = read_file_lines("input.txt");

    if (lines.empty()) {
        std::cout << "Error opening file" << std::endl;
        return 0;
    }

    std::vector<int> left;
    std::vector<int> right;

    for (auto& line : lines) {
        auto ints = parse_ints(line, "   ");
        left.push_back(ints.front());
        right.push_back(ints.back());
    }

    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());
    std::vector<int> distance;

    for (int i = 0; i < left.size(); i++)
        answer1 += std::abs(right[i] - left[i]); 

    for (auto& c : left)
        answer2 += std::count(right.begin(), right.end(), c) * c;

    std::cout << "Part 1 Answer : " << answer1 << std::endl;
    std::cout << "Part 2 Answer : " << answer2 << std::endl;
}
