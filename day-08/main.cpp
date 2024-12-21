#include "../common/util.h"

struct Antenna {
    char  freq;
    Coord pos;
};

using AntennaVector = std::vector<Antenna>;
using AntennaMap    = std::map<char, AntennaVector>;

void draw_antinode(const Coord& coord, TextGrid& grid) {
    if (!grid.out_of_bounds(coord))
        grid.at(coord) = '#';
}

void draw_antinodes(const AntennaVector& antennas, TextGrid& grid) {
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

void draw_antinodes2(AntennaVector& antennas, TextGrid& grid) {
    if (antennas.size() < 2)
        return;

    for (auto i = 0; i < antennas.size(); i++)
        for (auto j = i + 1; j < antennas.size(); j++) {
            auto a = std::min(antennas[i].pos, antennas[j].pos);
            auto b = std::max(antennas[i].pos, antennas[j].pos);

            auto delta = b - a;
            for (Coord pt = a; !grid.out_of_bounds(pt); pt = pt - delta)
                draw_antinode(pt, grid);
            for (Coord pt = a; !grid.out_of_bounds(pt); pt = pt + delta)
                draw_antinode(pt, grid);
    }
}

int main(int argc, char** argv) {
    auto answer1 = 0;
    auto answer2 = 0;

    auto animate = get_argument_flag("--animate", argc, argv);

    TextGrid grid = read_file_lines("input.txt");
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

    if (animate) {
        ansi::clear();
    }

    for (auto& v : antenna_map) {
        draw_antinodes(v.second, grid);
        if (animate) {
            ansi::home();
            grid.print();
            delayms(50);
        }
    }

    for (auto& c : grid)
        answer1 += c == '#';

    for (auto& v : antenna_map) {
        draw_antinodes2(v.second, grid);
        if (animate) {
            ansi::home();
            grid.print();
            delayms(50);
        }
    }

    std::cout << "Answer Part 1: " << answer1 << std::endl;
    //std::cout << "Answer Part 2: " << answer2 << std::endl;
}