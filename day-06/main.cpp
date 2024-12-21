#include "../common/util.h"

struct Guard
{
    TextGrid& grid;
    Coord north = {-1,  0}; Coord south = { 1,  0};
    Coord east  = { 0,  1}; Coord west  = { 0, -1};
    Coord direction[4] = {north, east, south, west};
    Coord current, start;
    int state;

    std::set<Coord> visited;

public:
    Guard(TextGrid& grid)
        : grid(grid)
     {
        current = start = grid.find_first('^');
        if (start == Coord())
            throw std::runtime_error("unable to find guard");
    }

    Coord next() {
        return current + direction[state];
    }
    
    void forward() {
        grid.set(next(), grid.get(current));
        grid.set(current, ' ');
        current = next();
        visited.insert(current);
    }

    void turn() { ++state &= 3; }

    bool is_blocked() { return grid.at(next()) == '#'; }

    bool is_exiting() { return grid.out_of_bounds(next()); }

    bool is_looping() { return visited.contains(current) && visited.contains(next()); }

    void simulate(bool animate) {
        visited.clear();
        visited.insert(current);
        ansi::clear();

        while (!grid.out_of_bounds(current)) {
            ansi::home();
            if (animate) {
                grid.zoom_print(current, 50);
                std::cout << std::endl;
                delayms(50);
            }

            if (grid.out_of_bounds(next()))
                return;

            if (grid.get(next()) == '#')
                turn();

            forward();
        }
    }

};


int main(int argc, char** argv) {
    auto animate = get_argument_flag("--animate", argc, argv);

    TextGrid lines = read_file_lines("input.txt");
    if (lines.empty()) {
        std::cout << "unable to load input" << std::endl;
        return 0;
    }

    auto grid = TextGrid(lines);
    auto guard = Guard(grid);

    guard.simulate(animate);
    int64_t answer1 = guard.visited.size();

    std::cout << "Answer Part 1: " << answer1 << std::endl;
    //std::cout << "Answer Part 2: " << answer2 << std::endl;
}