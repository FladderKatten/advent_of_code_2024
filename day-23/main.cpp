#include "../common/util.h"

struct Solver
{
    std::vector<std::string> computers;
    std::vector<std::pair<std::string,std::string>> pairs;
    std::map<std::string,std::set<std::string>> connections;

    Solver() {}

    Solver(const std::vector<std::string>& lines)
    {
        auto unique = std::set<std::string>();
        auto pairs = std::vector<std::pair<std::string,std::string>>();

        for (auto line : lines) {
            auto tokens = tokenize(line, "-");
            assert(tokens.size() == 2);
            pairs.emplace_back(tokens.front(), tokens.back());
        }

        for (auto& [a, b] : pairs) {
            connections[a].insert(b);
            connections[b].insert(a);
            unique.insert(a);
        }

        computers = std::vector<std::string>(unique.begin(), unique.end());

        std::sort(pairs.begin(), pairs.end());
        std::sort(computers.begin(), computers.end());
    }

    std::set<std::vector<std::string>> find_triplets() {
        auto groups = std::set<std::vector<std::string>>();

        for (const auto& [a, neighbors] : connections) {
            std::vector<std::string> candidates(neighbors.begin(), neighbors.end());

            for (size_t i = 0; i < candidates.size(); i++)
                for (size_t j = i + 1; j < candidates.size(); j++) {
                    const std::string& b = candidates[i];
                    const std::string& c = candidates[j];

                    if (connections[b].count(c) && connections[c].count(b)) {
                        std::vector<std::string> group = {a, b, c};
                        std::sort(group.begin(), group.end());
                        groups.insert(group);
                    }
                }
        }
        return groups;
    }
};

void selftest() {

    auto triplets = Solver({"kh-tc","qp-kh","de-cg","ka-co","yn-aq","qp-ub","cg-tb","vc-aq","tb-ka","wh-tc",
    "yn-cg","kh-ub","ta-co","de-co","tc-td","tb-wq","wh-td","ta-ka","td-qp","aq-cg","wq-ub","ub-vc",
    "de-ta","wq-aq","wq-vc","wh-yn","ka-de","kh-ta","co-tc","wh-qp","tb-vc","td-yn",}).find_triplets();

    assert(triplets.size() == 12);

    auto ts = std::count_if(triplets.begin(), triplets.end(), [](const auto& v) {
        return std::any_of(v.begin(), v.end(), [](const auto& s) { return s.starts_with("t"); });
    });

    assert(ts == 7);

}

int main() {
    selftest();

    auto lines = read_file_lines("input.txt");
    if (lines.empty())
        throw std::runtime_error("no input");

    Solver solver(lines);
    auto triplets = solver.find_triplets();

    auto answer1 = std::count_if(triplets.begin(), triplets.end(), [](const auto& v) {
        return std::any_of(v.begin(), v.end(), [](const auto& s) { return s.starts_with("t"); });
    });

    std::cout << "Answer Part 1: " << answer1 << std::endl;
}
