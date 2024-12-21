#include "../common/util.h"

struct Robot
{
    Coord start;
    Coord position;
    TextGrid& grid;

public:
    Robot(TextGrid& grid)
        : grid(grid)
     {
        start = position = grid.find_first('@');
        if (start == Coord(-1, -1))
            throw std::runtime_error("unable to find start location");
    }

    void push(Coord from, Coord delta) {
        auto dest = from + delta;

        if (grid.out_of_bounds(dest))
            return;

        if (grid.at(dest) == 'O')
            push(dest, delta);

        if (grid.at(dest) == '.') {
            grid.at(dest) = grid.at(from);
            grid.at(from) = '.';
        }
    }

    void execute_instruction(char instr) {
        Coord delta;
        
        switch (instr) {
            case '<':   delta = Coord( 0, -1); break;
            case '>':   delta = Coord( 0, +1); break;
            case '^':   delta = Coord(-1,  0); break;
            case 'v':   delta = Coord(+1,  0); break;
            default:    return;
        }

        push(position, delta);
        
        if (grid.at(position) != '@')
            position = position + delta;
    }

    void run(const std::string& code, int delay = 50) {
        char i;
        auto instuctions = code;

        auto print_code = [&]() {
            auto len = std::min(instuctions.length(), size_t(30));
            std::cout << "[" << i << "] " << instuctions.substr(0, len) << "  " << std::endl;
        };

        ansi::clear_screen();
        ansi::hide_cursor();

        while (!instuctions.empty()) {
            ansi::home();
            grid.print();
            std::cout << "\n Code : ";
            print_code();
            std::cout << " ";
            i = instuctions.front();
            instuctions.erase(0, 1);
            execute_instruction(i);
            std::cout << std::endl;

            //std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        }
    }

    int64_t checksum_boxes() {
        int64_t sum = 0;

        for (int i = 0; i < grid.height; i++)
            for (int j = 0; j < grid.width; j++)
                if (grid.at(i, j) == 'O') {
                    sum += i * 100 + j;
                }

        return sum;
    }
};


int main() {
    int64_t answer1 = 0;
    int64_t answer2 = 0;

    auto lines = read_file_lines("input.txt");
    if (lines.empty()) {
        std::cout << "unable to load input" << std::endl;
        return 0;
    }

    auto empty_line = std::find_if(lines.begin(), lines.end(), [](auto& line) { return line.empty(); });
    
    auto warehouse = std::vector<std::string>();
    std::copy(lines.begin(), empty_line, std::back_inserter(warehouse));

    auto code = std::string();
    std::for_each(empty_line + 1, lines.end(), [&](const std::string& s) {
            code += s;
        });

    auto grid = TextGrid(warehouse);
    auto robot = Robot(grid);

    robot.run(code, 10);

    answer1 = robot.checksum_boxes();

    std::cout << "Answer Part 1: " << answer1 << std::endl;
    //std::cout << "Answer Part 2: " << answer2 << std::endl;
}
