#include "../common/util.h"

using Sequence = std::vector<int>;

struct Buyer
{
    int64_t i;
    int64_t secret;
    std::vector<int64_t> offers;
    std::vector<int64_t> changes;
    std::map<Sequence, int64_t> sequences;
    
    Buyer(const std::string& secret)
        : Buyer(std::stoull(secret)) {}

    Buyer(const int64_t secret)
        : secret(secret)
        , offers({secret%10})
        , changes({0}) {}

    void mix(int64_t value) { secret ^= value; };

    void prune()            { secret %= 16777216; };

    int64_t advance(int times) {
        while (times--) {
            mix(secret * 64);
            prune();
            mix(secret / 32);
            prune();
            mix(secret * 2048);
            prune();

            auto offer = secret % 10;

            changes.push_back(offer - offers.back());
            offers.push_back(offer);

            if (offers.size() >= 4) {
                auto seq = Sequence(changes.end()-4, changes.end());
                if (!sequences.contains(seq)) {
                    sequences[seq] = offers.back();
                }
            }

        }
        return secret;
    }
};

struct Solver
{
    std::vector<Buyer> buyers;

    Solver(const std::vector<std::string>& lines) {
        std::copy(lines.begin(), lines.end(), std::back_inserter(buyers));
    }

    int64_t solve_answer1(int times) {
        return std::accumulate(buyers.begin(), buyers.end(), 0LL, [&](int64_t acc, Buyer& b) {
            b.advance(times);
            return acc + b.secret;
        });
    }

    int64_t solve_answer2() {
        auto rankings = std::map<Sequence, int64_t>();

        for (auto& b : buyers)
            for (auto& [seq, price] : b.sequences)
                rankings[seq] += price;

        auto sorted = std::vector<std::pair<Sequence, int64_t>>(rankings.begin(), rankings.end());
        std::sort(sorted.begin(), sorted.end(),
              [](const auto& a, const auto& b) {
                  return a.second > b.second;
              });

        return sorted.front().second;
    }
};

void selftest() {
    assert(Solver({"1", "10", "100", "2024"}).solve_answer1(1999) == 34050623);
    auto b = Buyer(123);
    assert(b.advance(1) == 15887950);
    assert(b.advance(1) == 16495136);
    assert(b.advance(1) == 527345);
    assert(b.advance(1) == 704524);
    assert(b.advance(1) == 1553684);
    assert(b.advance(1) == 12683156);
    assert(b.advance(1) == 11100544);
    assert(b.advance(1) == 12249484);
    assert(b.advance(1) == 7753432);
    assert(b.advance(1) == 5908254);
}

int main() {
    selftest();

    auto lines = read_file_lines("input.txt");
    if (lines.empty())
        throw std::runtime_error("no input");

    Solver solver(lines);

    auto answer1 = solver.solve_answer1(2000);
    auto answer2 = solver.solve_answer2();
    std::cout << "Answer Part 1: " << answer1 << std::endl;
    std::cout << "Answer Part 2: " << answer2 << std::endl;
}
