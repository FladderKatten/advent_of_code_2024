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

class String;

using StringVector = std::vector<String>;
using IntVector    = std::vector<int>;

class String
    : public std::string
{
public:
    using std::string::string;

    String(const std::string& s) : std::string(s) {}

    StringVector tokenize(char delimiter) {
        auto tokens = StringVector();

        size_t i = 0, end;
        while ((end = find(delimiter, i)) != npos) {
            tokens.emplace_back(substr(i, end - i));
            i = end + 1;
        }

        tokens.emplace_back(substr(i));
        return tokens;
    }

    IntVector parse_ints(char delimiter) {
        auto numbers = std::vector<int>();
        
        for (const auto& token : tokenize(delimiter)) {
            numbers.push_back(std::stoi(token));
        }

        return numbers;
    }

};

String read_file_raw_text(const std::string& filename) {
    auto f    = std::ifstream(filename);
    auto text = String();

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
}
