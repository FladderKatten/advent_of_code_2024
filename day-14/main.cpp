#include "../common/util.h"

struct Robot
{
	TextGrid& grid;
	Coord velocity;
	Coord position;
	
	Robot(TextGrid& grid, Coord position, Coord velocity)
		: grid(grid), position(position), velocity(velocity)
	{
		position.normalize(grid.height, grid.width);
	}

	void move() {
		position = position + velocity;
		position.normalize(grid.height, grid.width);
	}
};

std::vector<Robot> parse(const std::string& text, TextGrid& grid) {
    auto robots = std::vector<Robot>();

    auto pattern = std::regex(R"(p=(\d+),(\d+)\s*v=(-?\d+),(-?\d+))");
    auto match = std::smatch();

    auto begin = std::sregex_iterator(text.begin(), text.end(), pattern);
    auto end = std::sregex_iterator();

    for (std::sregex_iterator i = begin; i != end; i++) {
        match = *i;
		if (match.size() != 5)
            throw std::invalid_argument("invalid input");

        Coord pos = { std::stoi(match[2].str()), std::stoi(match[1].str()) };
        Coord vel = { std::stoi(match[4].str()), std::stoi(match[3].str()) };

        robots.emplace_back(grid, pos, vel);
    }

    return robots;
}

int main() {
	auto text = read_file_raw_text("input.txt");
	if (text.empty()) {
		std::cout << "unable to load input" << std::endl;
		return 0;
	}

	auto grid = TextGrid(103, 101);
	auto robots = parse(text, grid);

	for (int i = 0; i < 100; i++)
		for (auto& r : robots)
			r.move();

	
	int mid_row = grid.height/2;
	int mid_col = grid.width/2;
	std::array<int, 4> quad = {0};

	for (auto& r : robots) {
		if (r.position.row == 51 || r.position.col == 50)
			continue;
		
		int i = (int(r.position.row > mid_row) * 2) + int(r.position.col > mid_col);
		quad[i]++;
	}

	int64_t answer1 = quad[0] * quad[1] * quad[2] * quad[3];

    std::cout << "Answer Part 1: " << answer1 << std::endl;
    //std::cout << "Answer Part 2: " << answer2 << std::endl;
}