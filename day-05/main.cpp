#include "../common/util.h"

using Rule = IntVector;
using RuleVector = std::vector<Rule>;

using Update = IntVector;
using UpdateVector = std::vector<Update>;

bool validate_update(const Update& update, const RuleVector& rules) {
	// validate every rule
	for (auto& rule : rules) {

		// find the index of the pages
		auto i_first  = std::find(update.begin(), update.end(), rule.front());
		auto i_second = std::find(update.begin(), update.end(), rule.back());

		// skip if the update doesnt contain both rule pages
		if (i_first == update.end() || i_second == update.end())
			continue;
		// skip if correctly ordered
		if (i_first < i_second)
			continue;

		return false;
    }
	return true;
}

int main() {
	int answer1 = 0;
	int answer2 = 0;

	auto lines = read_file_lines("input.txt");
	if (lines.empty()) {
		std::cout << "unable to load input" << std::endl;
		return 0;
	}

	auto rules = RuleVector();
	auto updates = UpdateVector();
	
	// split the input into rules and updates
	auto found_empty_line = false;
	for (auto& line : lines) {
		if (line.empty())
			found_empty_line = true;
		else if (found_empty_line)
			updates.push_back(line.parse_ints(','));
		else
			rules.push_back(line.parse_ints('|'));
	};

	for (auto& u : updates) {
		if (validate_update(u, rules)) {
			answer1 += u[u.size()/2];
		}
	}

    std::cout << "Answer Part 1: " << answer1 << std::endl;
    //std::cout << "Answer Part 2: " << answer2 << std::endl;
}