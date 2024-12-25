#include "../common/util.h"

struct Shape
    : public std::vector<std::string>
{
public:
    using vector::vector;

    bool is_key()  const { return front() == "....." && back() == "#####"; };
    bool is_lock() const { return front() == "#####" && back() == "....."; };

    bool fits(const Shape& other) const {
        for (int i = 0; i < size(); i++)
            for (int j = 0; j < at(i).size(); j++)
                if (at(i).at(j) == '#' && other.at(i).at(j) == '#')
                    return false;
        return true;
    }
};

struct Solver
{
    std::vector<Shape> shapes;

    Solver() {}

    Solver(const std::vector<std::string>& lines)
    {
        for (auto it = lines.begin(); it != lines.end(); ) {
            auto end = std::find_if(it, lines.end(), [](auto& line) { return line.empty(); });
            shapes.emplace_back(it, end);
            it = (end != lines.end()) ? std::next(end) : end;
        }
    }

    auto count_pairs() {
        int64_t count = 0;

        auto keys  = std::vector<Shape>{};
        std::copy_if(shapes.begin(), shapes.end(), std::back_inserter(keys), [](const Shape& shape) { return shape.is_key(); });
        
        auto locks = std::vector<Shape>{};
        std::copy_if(shapes.begin(), shapes.end(), std::back_inserter(locks), [](const Shape& shape) { return shape.is_lock(); });

        while (!keys.empty()) {
            auto key = keys.back();
            keys.pop_back();

            count += std::count_if(locks.begin(), locks.end(), [&](const Shape& lock){
                return key.fits(lock);
            });
        }

        return count;
    }

    int64_t solve() {
        return count_pairs();
    }
};

int main() {
    auto lines = read_file_lines("input.txt");
    if (lines.empty())
        throw std::runtime_error("no input");

    Solver solver(lines);
    auto answer1 = solver.solve();
    std::cout << "Answer Part 1: " << answer1 << std::endl;
}
