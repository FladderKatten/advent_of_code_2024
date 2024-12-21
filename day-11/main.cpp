#include "../common/util.h"
struct Stone
{
    int64_t num;
public:
    Stone(const uint64_t n)
        : num(n) {}
    Stone(const std::string& s)
        : Stone(std::stoll(s)) {}

    std::pair<Stone,Stone> split() {
        auto d = digits();
        auto mid = d / 2;

        int64_t left = num / std::pow(10, d - mid);
        int64_t right = num % static_cast<int64_t>(std::pow(10, d - mid));

        return {left, right};
    }

    int digits()  const {
        return static_cast<int>(std::log10(num) + 1);
    }

    bool has_even_digits() { return digits() % 2 == 0; }

    operator int64_t () const { return num; }
};

using Key = std::pair<Stone, int64_t>;

std::map<Key, int64_t> cache;

int64_t trace(Stone stone, int times) {
    if (times == 0)
        return 0;
    
    if (cache.find({stone, times}) != cache.end())
        return cache[{stone, times}];

    int64_t result = 0;

    if (stone.num == 0)
        result = trace(1, times - 1);
    else if (stone.has_even_digits()) {
        auto [left, right] = stone.split();
        result  = 1;
        result += trace(left,  times - 1)
               +  trace(right, times - 1);
    } else
        result = trace(stone * 2024, times - 1);

    cache[{stone, times}] = result;
    
    return result;
}

auto blink(const std::vector<Stone>& stones, int times) {
    return std::accumulate(stones.begin(), stones.end(), 0LL, [times](auto acc, auto stone) {
        return acc + 1 + trace(stone, times);
    });
}

void selftest() {
    assert(blink({125,17}, 6) == 22);
    assert(blink({125,17}, 25) == 55312);
}

int main(int argc, char** argv) {
    auto text = read_file_lines("input.txt");
    if (text.empty()) {
        std::cout << "Failed to read input file" << std::endl;
        return 1;
    }
   
    auto tokens = parse_ints(text.front(), " ");
    auto stones = std::vector<Stone>(tokens.begin(), tokens.end());

    int64_t answer1 = blink(stones, 25);
    std::cout << "Answer Part 1: " << answer1 << std::endl;
    int64_t answer2 = blink(stones, 75);
    std::cout << "Answer Part 2: " << answer2 << std::endl;
}
