#include "../common/util.h"
#include <regex>

struct Point {
	int64_t x, y;
};

struct Machine {

	Point a, b, prize;

    Machine(Point a, Point b, Point prize)
		: a(a), b(b), prize(prize) {}

    int64_t bruteforce() const {
		auto max_presses = [&](const Point& button) {
			auto value = std::max(prize.x / button.x, prize.y / button.y) + 1;
			return std::min(value, 100L);
		};

		auto max_a = max_presses(a);
		auto max_b = max_presses(b);

		int64_t best = INT64_MAX;
        for (int64_t a_presses = 0; a_presses < max_a; a_presses++) {
	        for (int64_t b_presses = 0; b_presses < max_b; b_presses++) {
				Point pos = {a_presses * a.x + b_presses * b.x,
							 a_presses * a.y + b_presses * b.y };

				if (pos.x != prize.x || pos.y != prize.y)
					continue;

				auto cost = a_presses*3 + b_presses;
				best = std::min(best, cost);
			}
 	   	}
		
		if (best == INT64_MAX)
    		return 0;

		return best;
	}
};

std::vector<Machine> parse(const std::string& text) {
    auto machines = std::vector<Machine>();

    auto pattern = std::regex(R"(Button A: X\+(\d+), Y\+(\d+)\s*Button B: X\+(\d+), Y\+(\d+)\s*Prize: X=(\d+), Y=(\d+))");
    auto match = std::smatch();

    auto begin = std::sregex_iterator(text.begin(), text.end(), pattern);
    auto end = std::sregex_iterator();

    for (std::sregex_iterator i = begin; i != end; ++i) {
        match = *i;
		
		if (match.size() != 7)
            throw std::invalid_argument("invalid input");

        Point a =     { std::stoll(match[1].str()), std::stoll(match[2].str()) };
        Point b =     { std::stoll(match[3].str()), std::stoll(match[4].str()) };
        Point prize = { std::stoll(match[5].str()), std::stoll(match[6].str()) };

        machines.push_back(Machine(a, b, prize));
    }

    return machines;
}



void self_test() {
	assert(Machine({94, 34},{22, 67},{8400, 5400}).bruteforce() == 280);
	assert(Machine({17, 86},{84, 37},{7870, 6450}).bruteforce() == 200);
}

int main() {
	self_test();

	auto text = read_file_raw_text("input.txt");
	if (text.empty()) {
		std::cout << "unable to load input" << std::endl;
		return 0;
	}

	auto machines = parse(text);

	int answer1 = 0;
	for (const auto& m : machines) {
		auto tokens = m.bruteforce();
		answer1 += tokens;
	}

    std::cout << "Answer Part 1: " << answer1 << std::endl;
    //std::cout << "Answer Part 2: " << answer2 << std::endl;
}