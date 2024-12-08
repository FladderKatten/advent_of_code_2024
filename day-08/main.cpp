#include "../common/util.h"


struct Coord {
    int row, col;

    Coord(int row = 0, int col = 0)
        : row(row), col(col) {}

    bool  operator< (const Coord& other) const { return row != other.row? row < other.row : col < other.col; }
    bool  operator> (const Coord& other) const { return row != other.row? row > other.row : col > other.col; }
    bool  operator==(const Coord& other) const { return row == other.row && col == other.col; }
    Coord operator+ (const Coord& other) const { return { row + other.row, col + other.col }; }
    Coord operator- (const Coord& other) const { return { row - other.row, col - other.col }; }
};

class Grid
    : public StringVector
{
public:
    int height;
    int width;

    Grid(const StringVector& sv)
        : StringVector(sv)
        , height(sv.size())
        , width(sv.front().size()) {}

    char& at(const Coord& c) { return StringVector::at(c.row).at(c.col); }

    void print(int ms_delay) const {
        ansi::clear();
        auto s = std::string();

        for (auto& row : *this)
            s += row + "\n";
        std::cout << s << std::endl;
        delayms(ms_delay);
    }
};

struct Antenna {
    char  freq;
    Coord pos;
};

using AntennaVector = std::vector<Antenna>;
using AntennaMap    = std::map<char, AntennaVector>;

bool validate_coord(const Coord& coord, const Grid& grid) {
    return coord.row >= 0 && coord.row < grid.height
        && coord.col >= 0 && coord.col < grid.width;
}

void draw_antinode(const Coord& coord, Grid& grid) {
    if (validate_coord(coord, grid))
        grid.at(coord) = '#';
}

void draw_antinodes(const AntennaVector& antennas, Grid& grid) {
    if (antennas.size() < 2)
        return;

    for (auto i = 0; i < antennas.size(); i++)
        for (auto j = i + 1; j < antennas.size(); j++) {
        auto a = antennas[i].pos;
        auto b = antennas[j].pos;

        Coord delta = b - a;
        draw_antinode(a - delta, grid);
        draw_antinode(b + delta, grid);
    }
}

void draw_antinodes2(AntennaVector& antennas, Grid& grid) {
    if (antennas.size() < 2)
        return;

    for (auto i = 0; i < antennas.size(); i++)
        for (auto j = i + 1; j < antennas.size(); j++) {
            auto a = std::min(antennas[i].pos, antennas[j].pos);
            auto b = std::max(antennas[i].pos, antennas[j].pos);

            auto delta = b - a;
            for (Coord pt = a; validate_coord(pt, grid); pt = pt - delta)
                draw_antinode(pt, grid);
            for (Coord pt = a; validate_coord(pt, grid); pt = pt + delta)
                draw_antinode(pt, grid);
    }
}

int main(int argc, char** argv) {
    auto answer1 = 0;
    auto answer2 = 0;

    auto animate = get_argument_flag("--animate", argc, argv);

    Grid grid = read_file_lines("input.txt");
    if (grid.empty()) {
        std::cout << "unable to load input" << std::endl;
        return 0;
    }

    auto antennas = AntennaVector();
    auto antenna_map = AntennaMap();

    for (int i = 0; i < grid.height; i++)
        for (int j = 0; j < grid.width; j++) {
            auto coord = Coord(i, j);
            if (grid.at(coord) != '.') {
                auto a = Antenna{grid.at(coord), coord};
                antennas.push_back(a);
                antenna_map[grid.at(coord)].push_back(a);
            }
        }

    for (auto& v : antenna_map) {
        draw_antinodes(v.second, grid);
        if (animate) {
            grid.print(50);
        }
    }

    for (auto& line : grid)
        answer1 += std::count(line.begin(), line.end(), '#');

    for (auto& v : antenna_map) {
        draw_antinodes2(v.second, grid);
        if (animate) 
            grid.print(50);
    }

    for (auto& line : grid)
        answer2 += std::count(line.begin(), line.end(), '#');

    std::cout << "Answer Part 1: " << answer1 << std::endl;
    std::cout << "Answer Part 2: " << answer2 << std::endl;
}