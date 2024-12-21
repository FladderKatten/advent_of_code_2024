#pragma once
#include <algorithm>
#include <numeric>
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
#include <regex>
#include <cmath>
#include <climits>
#include <iomanip>

std::vector<std::string> tokenize(const std::string& s, const std::string& delimiter) {
    auto tokens = std::vector<std::string>();
    size_t i = 0, end;
    while ((end = s.find(delimiter, i)) != s.npos) {
        tokens.emplace_back(s.substr(i, end - i));
        i = end + delimiter.size();
    }
    tokens.emplace_back(s.substr(i));
    return tokens;
}

std::vector<int> parse_ints(const std::string& s, const std::string& delimiter) {
       auto numbers = std::vector<int>();
       
       for (auto& token : tokenize(s, delimiter))
           numbers.push_back(std::stoi(token));

       return numbers;
}

std::string read_file_raw_text(const std::string& filename) {
    std::ifstream f(filename, std::ios::binary);
    if (!f.is_open())
        throw std::runtime_error("Could not open file: " + filename);
    return std::string(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
}

std::vector<std::string> read_file_lines(const std::string& filename) {
    auto f  = std::ifstream(filename);
    auto sv = std::vector<std::string>();

    if (f.is_open())
        for (std::string line; std::getline(f, line); )
            sv.emplace_back(line);

    return sv;
}

void delayms(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

bool get_argument_flag(const std::string& name, int argc, char** argv) {
    for (int i = 1; i < argc; i++)
        if (name == std::string(argv[i]))
            return true;

    return false;
}

struct Coord
{
    int row;
    int col;

    Coord(const int row, const int col)
        : row(row), col(col) {}

    Coord()
        : row(INT_MAX), col(INT_MAX) {}

    bool is_cross_adjecent(const Coord& other) const {
        return (std::abs(row - other.row) == 1 && col == other.col)
            || (std::abs(col - other.col) == 1 && row == other.row);
    }

    void normalize(int rows, int columns) {
        row = (row % rows + rows) % rows;
        col = (col % columns + columns) % columns;
    }

    Coord up(int n = 1)    const { return *this + Coord(0,-n); }
    Coord down(int n = 1)  const { return *this + Coord(0,+n); }
    Coord left(int n = 1)  const { return *this + Coord(-n,0); }
    Coord right(int n = 1) const { return *this + Coord(+n,0); }

    bool  operator< (const Coord& other) const { return row < other.row || (row == other.row && col < other.col); }
    bool  operator> (const Coord& other) const { return row > other.row || (row == other.row && col > other.col); }
    bool  operator==(const Coord& other) const { return row == other.row && col == other.col; }
    bool  operator!=(const Coord& other) const { return row != other.row || col != other.col; }
    Coord operator+ (const Coord& other) const { return { row + other.row, col + other.col }; }
    Coord operator- (const Coord& other) const { return { row - other.row, col - other.col }; }
};


class TextGrid
{
public:
    std::vector<std::string> lines;
    int   height;
    int   width;

    TextGrid(int height, int width)
        : height(height)
        , width(width)
        , lines(height, std::string(width, ' ')) {}

    TextGrid(const std::vector<std::string>& lines)
        : height(lines.size())
        , width(lines[0].size())
        , lines(lines) {}

    class Iterator {
        TextGrid& grid;
        Coord     pos;
    public:
        Iterator(TextGrid& grid, const Coord pos)
            : grid(grid), pos(pos) {}

        char& operator*() { return grid.at(pos); }

        Iterator operator++() {
            if (++pos.col == grid.width)
                pos = Coord(pos.row + 1, 0);
            return *this;
        }

        Iterator operator++(int) {
            auto tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const Iterator& other) const { return pos == other.pos; }
        bool operator!=(const Iterator& other) const { return pos != other.pos; }
    };

    Iterator begin() { return Iterator(*this, Coord(0,0)); }
    Iterator end()   { return Iterator(*this, Coord(height, 0)); }

    virtual void print() {
        for (auto& line : lines)
            std::cout << line << "\n";
    }

    Coord find_first(const char ch) {
        for (int row = 0; row < lines.size(); ++row)
            for (int col = 0; col < lines[row].size(); ++col)
                if (lines[row][col] == ch)
                    return { row, col };
        
        return Coord();
    }

    bool empty() {
        return std::all_of(lines.begin(), lines.end(), [](const std::string& line) { return line.empty(); });
    }

    std::string& line(const int row) {
        return lines[row];
    }

    bool out_of_bounds(const Coord& pos) {
        return out_of_bounds(pos.row, pos.col);
    }

    bool out_of_bounds(const int row, const int col) {
        return row < 0 || row >= lines.size() || col < 0 || col >= lines[row].size();
    }

    void set(const Coord& pos, char ch) { at(pos) = ch;  }
    char get(const Coord& pos)          { return at(pos); }
    char& at(const Coord& pos)          { return at(pos.row, pos.col); }

    char& at(const int row, const int col) {
        assert(!out_of_bounds(row, col));
        return lines[row][col];
    }

    void zoom_print(const Coord& center, int zoom_size) const {
        int half_rows = zoom_size / 2;
        int half_cols = zoom_size / 2;

        int start_row = center.row - half_rows;
        int end_row = center.row + half_rows + 1;

        int start_col = center.col - half_cols;
        int end_col = center.col + half_cols + 1;

        if (start_row < 0) {
            start_row = 0;
            end_row = std::min(height, zoom_size);
        } else if (end_row > height) {
            end_row = height;
            start_row = std::max(0, height - zoom_size);
        }

        if (start_col < 0) {
            start_col = 0;
            end_col = std::min(width, zoom_size);
        } else if (end_col > width) {
            end_col = width;
            start_col = std::max(0, width - zoom_size);
        }

        for (int i = start_row; i < end_row; ++i) {
            for (int j = start_col; j < end_col; ++j)
                std::cout << lines[i][j];

            std::cout << '\n';
        }
    }

};


namespace ansi {
    struct Color {
        int r, g, b;
        Color(int r, int g, int b)
            : r(r), g(g), b(b) {}
    };

    void hide_cursor()         { std::cout << "\033[?25l"; }
    void save_cursor()         { std::cout << "\033[s"; }
    void restore_cursor()      { std::cout << "\033[u"; }
    void clear()               { std::cout << "\033[2J\033[H"; }

    void move_cursor(int row, int col) {
        std::cout << "\033[" << row << ";" << col << "H";
    }

    void print_at(int row, int col, std::string_view str) {
        move_cursor(row, col);
        std::cout << "\033[", row, ";", col, "f";
        std::cout << str;
    }
    
    static void home()         { std::cout << "\033[H"; }
    static void clear_screen() { std::cout << "\033[2J"; }
    static void reset_all()    { std::cout << "\033[0m"; }
}

std::ostream &operator<<(std::ostream &os, const ansi::Color& color) {
    os << "\033[38;2;" << color.r << ';' << color.g << ';' << color.b << 'm';
    return os;
}

