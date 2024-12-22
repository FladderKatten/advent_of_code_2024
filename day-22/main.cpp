#include "../common/util.h"

struct Buyer
{
    int64_t secret;

    Buyer(const std::string& secret)
        : Buyer(std::stoull(secret)) {}

    Buyer(const int64_t secret)
        : secret(secret) {}

    void mix(int64_t value) { secret ^= value; }
    void prune()            { secret %= 16777216;  }

    int64_t advance(int times) {
        while (times--) {
            mix(secret * 64);
            prune();
            mix(secret / 32);
            prune();
            mix(secret * 2048);
            prune();
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

    int64_t solve(int times) {
        return std::accumulate(buyers.begin(), buyers.end(), 0LL, [&](int64_t acc, Buyer& b) {
            b.advance(times);
            return acc + b.secret;
        });
    }
};

void selftest() {
    assert(Solver({"1", "10", "100", "2024"}).solve(1999) == 34050623);
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

    auto answer1 = solver.solve(2000);
    std::cout << "Answer Part 1: " << answer1 << std::endl;
}
