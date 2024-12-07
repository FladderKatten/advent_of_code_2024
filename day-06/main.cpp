#include "../common/util.h"

struct Coord {
	int row, col;

	Coord(int row = 0, int col = 0)
		: row(row), col(col) {}

      bool operator<(const Coord& other) { return row != other.row? row < other.row : col < other.col; }
      bool operator>(const Coord& other) { return row != other.row? row > other.row : col > other.col; }
	Coord operator==(const Coord& other) { return row == other.row && col == other.col; }
	Coord operator+ (const Coord& other) { return {row + other.row,   col  + other.col}; }
};

class Grid
	: public StringVector
{
public:
	Grid(const StringVector& sv)
		: StringVector(sv) {}

	char& at(const Coord& c) { return StringVector::at(c.row).at(c.col); }

	std::map<Coord, bool> visited;
};

struct Guard
	: public Coord
{
	Coord north = {-1,  0};	Coord south = { 1,  0};
	Coord east  = { 0,  1};	Coord west  = { 0, -1};
	Coord direction[4] = {north, east, south, west};
	Coord pos;
	Grid& grid;
	int state;

public:
	Guard(Grid& grid)
		: grid(grid)
	 {
		// find the start location
		for (pos.row = 0; pos.row < grid.size(); pos.row++)
			for (pos.col = 0; pos.col < grid[pos.row].size(); pos.col++) {
				if (grid.at(pos) == '^') {
					std::cout << "found guard at " << pos.row << ", " << pos.col << std::endl;
					return;
				}
			}
	}

	Coord next_location() { return pos + direction[state]; }
	
	void move() { pos = next_location(); }

	void turn() { ++state &= 3; }

	bool is_blocked() { return grid.at(next_location()) == '#';	}

	bool is_exiting() {	return next_location().row < 0 || next_location().row >= grid.size()
							|| next_location().col < 0 || next_location().col >= grid.front().size(); }

};


int main() {
	int answer1 = 0;
	int answer2 = 0;

	Grid grid = read_file_lines("input.txt");
	if (grid.empty()) {
		std::cout << "unable to load input" << std::endl;
		return 0;
	}

	int rows = grid.size();
	int cols = grid.front().size();

	auto guard = Guard(grid);

	auto visited = std::set<std::pair<int, int>>();
	visited.insert({guard.pos.row, guard.pos.col});

	while (!guard.is_exiting()) {
		if (guard.is_blocked())
			guard.turn();

		guard.move();
		visited.insert({guard.pos.row, guard.pos.col});
	}

	answer1 = visited.size();

   	std::cout << "Answer Part 1: " << answer1 << std::endl;
    //std::cout << "Answer Part 2: " << answer2 << std::endl;
}