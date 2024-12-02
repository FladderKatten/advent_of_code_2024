#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>



int main() {

    std::ifstream f("input.txt");

    if (!f.is_open()) {
        std::cout << "Error opening file" << std::endl;
        return 0;
    }

    std::vector<int> left;
    std::vector<int> right;

    for (std::string line; std::getline(f, line); ) {
        std::istringstream ss(line);

        int a, b;

        ss >> a >> b;

        left.push_back(a);
        right.push_back(b);

        // std::cout << a << " - " << b << std::endl; //
    }
    f.close();



    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());
    std::vector<int> distance;

    for (int i = 0; i < left.size(); i++) {
        distance.push_back(std::abs(right[i] - left[i])); 
    }       


    int total = 0;
    for (auto& d : distance) {
        total += d;
    }


    std::cout << "Total distance: " << total << std::endl;

}





