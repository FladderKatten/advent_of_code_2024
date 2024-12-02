#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include <array>


class Report
{
    std::vector<int> levels;
public:
    Report(std::istringstream &ss) {
        int n;
        while (ss >> n)
            levels.push_back(n);
    }

    bool is_safe_dec() {
        for (int i = 1; i < levels.size(); i++) {
            int d = levels[i-1] - levels[i];
            if (d < 1 || d > 3)
                return false;
        }
        return true;
    }

    bool is_safe_inc() {
        for (int i = 1; i < levels.size(); i++) {
            int d = levels[i] - levels[i-1];
            if (d < 1 || d > 3)
                return false;
        }
        return true;
    }

    bool is_safe() {
        return is_safe_dec() || is_safe_inc();
    }
};


int main() {

    std::ifstream f("input.txt");

    if (!f.is_open()) {
        std::cout << "Error opening file" << std::endl;
        return 0;
    }

    int safe_reports = 0;
    for (std::string line; std::getline(f, line); ) {
        std::istringstream ss(line);

        Report rep(ss);

        safe_reports += rep.is_safe();
    }
    f.close();

    std::cout << "Answer: " << safe_reports << std::endl;


}





