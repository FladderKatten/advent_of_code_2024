#pragma once
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <iterator>
#include <inttypes.h>
#include <assert.h>
#include <thread>
#include <chrono>

using StringVector = std::vector<std::string>;
using IntVector    = std::vector<int>;

StringVector tokenize(const std::string& s, const std::string& delimiter) {
    auto tokens = StringVector();
    size_t i = 0, end;
    while ((end = s.find(delimiter, i)) != s.npos) {
        tokens.emplace_back(s.substr(i, end - i));
        i = end + delimiter.size();
    }
    tokens.emplace_back(s.substr(i));
    return tokens;
}

IntVector parse_ints(const std::string& s, const std::string& delimiter) {
       auto numbers = std::vector<int>();
       
       for (auto& token : tokenize(s, delimiter))
           numbers.push_back(std::stoi(token));

       return numbers;
}

std::string read_file_raw_text(const std::string& filename) {
    auto f    = std::ifstream(filename);
    auto text = std::string();

    if (f.is_open()) {
	    std::copy(std::istream_iterator<char>(f), std::istream_iterator<char>(), std::back_inserter(text));
		f.close();
	}
	return text;
}

StringVector read_file_lines(const std::string& filename) {
    auto f  = std::ifstream(filename);
    auto ss = std::ostringstream();
    auto sv = StringVector();

    if (f.is_open()) {
        for (std::string line; std::getline(f, line); ) {
            sv.emplace_back(line);
        }
    }
    return sv;
}

namespace std {
    ostream& operator<<(ostream& os, const IntVector& vec) {
        bool first = true;
        for (auto& v : vec) {
            if (!first) {
                os << ", ";
            }
            os << v;
            first = false;
        }
        return os;
    }

    ostream& operator<<(ostream& os, const StringVector& vec) {
        bool first = true;
        for (auto& v : vec) {
            if (!first) {
                os << ", ";
            }
            os << "\"" << v << "\"";
            first = false;
        }
        return os;
    }
}


void msdelay(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

namespace ansi {
    void clear() { std::cout << "\033[2J\033[H"; }
}


bool get_argument_flag(const std::string& name, int argc, char** argv) {
    for (int i = 1; i < argc; i++)
        if (name == std::string(argv[i]))
            return true;

    return false;
}