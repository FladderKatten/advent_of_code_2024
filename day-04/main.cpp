
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iterator>

using Lines = std::vector<std::string>;
Lines lines;

Lines& read_lines() {
	auto f  = std::ifstream("input.txt");
    auto ss = std::ostringstream();

	if (f.is_open()) {
	    for (std::string line; std::getline(f, line); ) {
			lines.push_back(line);
    	}
	}
	return lines;
}

bool matchc(const char c, int row, int col) {
	if (row >= lines.size() || col >= lines[row].size() || row < 0 || col < 0)
		return false;
	return lines[row][col] == c;
}

bool match_direction(const std::string& word, int row, int col, int row_adder, int col_adder) {
	for (auto c : word) {
		if (matchc(c, row, col) == false)
			return false;
		row += row_adder;
		col += col_adder;
	}
	return true;
}

int find_all_from(const std::string& word, int row, int col) {
	int matches = 0;
	// match horizontal forward
	matches += match_direction(word, row, col,  0,  1);
	// match horizontal backward
	matches += match_direction(word, row, col,  0, -1);
	// match vertical down
	matches += match_direction(word, row, col,  1,  0);
	// match vertical up
	matches += match_direction(word, row, col, -1,  0);
	// match diagonally up left
	matches += match_direction(word, row, col, -1, -1);
	// match diagonally up right
	matches += match_direction(word, row, col, -1,  1);
	// match diagonally down left
	matches += match_direction(word, row, col,  1, -1);
	// match diagonally down right
	matches += match_direction(word, row, col,  1,  1);
	return matches;
}

bool match_x_mas(int row, int col) {
    if (match_direction("MAS", row-1, col-1,  1,  1) || match_direction("MAS", row+1, col+1,  -1,  -1))
	    if (match_direction("MAS", row+1, col-1,  -1,  1) || match_direction("MAS", row-1, col+1,  +1,  -1))
            return true;
    return false;
}

int main() {
	if (read_lines().empty()) {
		std::cout << "unable to read input" << std::endl;
		return 0;
	}

    /* debug
    lines = {
        "MMMSXXMASM",
        "MSAMXMSMSA",
        "AMXSXMAAMM",
        "MSAMASMSMX",
        "XMASAMXAMM",
        "XXAMMXXAMA",
        "SMSMSASXSS",
        "SAXAMASAAA",
        "MAMMMXMMMM",
        "MXMXAXMASX",
    };
    //*/

	auto answer1 = 0;
    for (int row = 0; row < lines.size(); row++) {
	    for (int col = 0; col < lines.size(); col++) {
			answer1 += find_all_from("XMAS", row, col);
    	}
    }


    auto answer2 = 0;
    for (int row = 0; row < lines.size(); row++) {
	    for (int col = 0; col < lines[row].size(); col++) {
			answer2 += match_x_mas(row, col);
    	}
    }

    std::cout << "Answer Part 1 : " << answer1 << std::endl;
    std::cout << "Answer Part 2 : " << answer2 << std::endl;
}
