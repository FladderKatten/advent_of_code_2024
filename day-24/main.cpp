#include "../common/util.h"

struct Operation
{
    std::pair<std::string, std::string> in;
    std::string op;
    std::string out;
    Operation(const std::pair<std::string, std::string>& in, const std::string& op, const std::string& out)
        : in(in)
        , out(out)
        , op(op) {}
};

struct Solver
{
    std::map<std::string, bool> values;
    std::vector<Operation>      operations;

    Solver() {}

    Solver(const std::vector<std::string>& lines)
    {
        auto empty_line = std::find_if(lines.begin(), lines.end(), [](auto& line) { return line.empty(); });
        assert(empty_line != lines.end());

        std::for_each(lines.begin(), empty_line, [&](const std::string& line) {
            auto tokens = tokenize(line, ": ");
            assert(tokens.size() == 2);
            values[tokens[0]] = std::stoi(tokens[1]);
        });

        std::for_each(empty_line+1, lines.end(), [&](const std::string& line) {
            auto tokens = tokenize(line, " ");
            assert(tokens.size() == 5);
            operations.emplace_back(std::make_pair(tokens[0], tokens[2]), tokens[1], tokens[4]);
        });
    }

    void execute(const Operation& o) {
        auto v1   = values[o.in.first];
        auto v2   = values[o.in.second];
        bool result;
        
        if (o.op == "AND")
            result = v1 && v2;
        else if (o.op == "OR")
            result = v1 || v2;
        else if (o.op == "XOR")
            result = v1 ^ v2;
        else
            throw std::runtime_error("unknown op " + o.op);
        
        values[o.out] = result;
    }

    void produce(const std::string& value) {
        for (auto& op : operations)
            if (op.out == value) {
                if (!values.contains(op.in.first))
                    produce(op.in.first);
                if (!values.contains(op.in.second))
                    produce(op.in.second);

            return execute(op);
        }
        throw std::runtime_error("unknown value " + value);
    }

    auto get_zwires() {
        auto zwires = std::vector<std::string>();
        for (auto& op : operations)
            if (op.out.starts_with("z"))
                zwires.emplace_back(op.out);

        return zwires;
    }

    int64_t solve() {
        auto zwires = get_zwires();
        std::sort(zwires.begin(), zwires.end(), std::greater());

        for (auto& name : zwires)
            produce(name);

        return std::accumulate(zwires.begin(), zwires.end(), 0LL, [&](auto acc, auto& s) {
            return acc*2 + values[s];
        });
    }
};

void test() {
    assert(Solver({
        "x00: 1", 
        "x01: 1", 
        "x02: 1", 
        "y00: 0", 
        "y01: 1", 
        "y02: 0", 
        "",
        "x00 AND y00 -> z00",
        "x01 XOR y01 -> z01",
        "x02 OR y02 -> z02"}).solve() == 4);
}

int main() {
    test();

    auto lines = read_file_lines("input.txt");
    if (lines.empty())
        throw std::runtime_error("no input");

    Solver solver(lines);
    auto answer1 = solver.solve();
    std::cout << "Answer Part 1: " << answer1 << std::endl;
}
