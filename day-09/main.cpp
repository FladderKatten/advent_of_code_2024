#include "../common/util.h"

static const int Free = ~0;

struct Block {
    int index;
    int id;
    int len;

    // todo fix for part2
    int64_t checksum() const {
        return id == Free
            ? 0
            : index * id;
    }
};

class DiskMap
{
    std::vector<Block> blocks;
public:
    DiskMap(const std::string& input) {
        int index = 0;
        for (int i = 0; i < input.length(); i++) {
            int id = i % 2? Free : i / 2;
            int len = input[i]-'0';
            blocks.push_back({index, id, len});
            index += len;
        }
    }

    DiskMap& expand() {
        auto expanded = std::vector<Block>();
        for (auto& b : blocks) {
            for (int i = 0; i < b.len; i++)
                expanded.push_back({b.index + i, b.id, 1});
        }
        blocks = expanded;
        return *this;
    }

    int64_t checksum() {
        return std::accumulate(
            blocks.begin(),
            blocks.end(),
            0LL,
            [&](int64_t sum, const Block& b) {
                return sum + b.checksum();
            });
    }

    DiskMap& part1() {
        for (int i = 0, j = blocks.size() - 1; i < j; ) {
            auto& a = blocks[i];
            auto& b = blocks[j];

            if (a.id != Free) {
                i++; continue;
            }

            if (b.id == Free) {
                j--; continue;
            }

            a = Block{i, b.id, 1};
            b = Block{j, Free, 1};
        }
        return *this;
    }
};

int main(int argc, char** argv) {
    auto animate = get_argument_flag("--animate", argc, argv);
    int64_t answer1 = 0;
    //int64_t answer2 = 0;

    auto text = read_file_raw_text("input.txt");
    if (text.empty()) {
        std::cout << "Failed to read input file" << std::endl;
        return 1;
    }

    assert(DiskMap("2333133121414131402").expand().part1().checksum() == 1928);

    auto disk1 = DiskMap(text);
    disk1.expand();
    disk1.part1();
    answer1 = disk1.checksum();

    std::cout << "Answer Part 1: " << answer1 << std::endl;
    //std::cout << "Answer Part 2: " << answer2 << std::endl;
}