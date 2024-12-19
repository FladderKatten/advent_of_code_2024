#include "../common/util.h"

struct VM {
    uint32_t ra, rb, rc;
    std::vector<int> code;
    std::string output;
    int ip;

    VM(const uint32_t ra, const uint32_t rb, const uint32_t rc, const std::vector<int> code)
        : code(code)
        , ra(ra), rb(rb), rc(rc)
        , ip(0) {}

    VM() {}

    enum Opcodes : int { OpcodeAdv, OpcodeBxl, OpcodeBst, OpcodeJnz, OpcodeBxc, OpcodeOut, OpcodeBdv, OpcodeCdv };

    uint32_t combo(char operand) {
        switch (operand) {
            case 0:
            case 1:
            case 2:
            case 3: return operand & 3;
            case 4: return ra;
            case 5: return rb;
            case 6: return rc;
            default: throw std::runtime_error("unknown operand");
        }
    }

    void single_step() {
        auto opcode  = code[ip++];
        auto operand = code[ip++];

        switch (opcode) {
            case OpcodeAdv: 
                ra >>= combo(operand);
                break;
            case OpcodeBxl:
                rb ^= operand;
                break;
            case OpcodeBst:
                rb = combo(operand) % 8;
                break;
            case OpcodeJnz:
                if (ra)
                    ip = operand;
                break;
            case OpcodeBxc:
                rb ^= rc;
                break;
            case OpcodeOut:
                output.push_back('0' + (combo(operand) % 8));
                break;
            case OpcodeBdv:
                rb = ra >> combo(operand);
                break;
            case OpcodeCdv:
                rc = ra >> combo(operand);
                break;
            default: throw std::runtime_error("Unknown opcode");
        }
    }

    VM& execute(int start = 0) {
        while (ip < code.size())
            single_step();

        return *this;
    }

    std::string output_csv() {
        std::string result;

        for (size_t i = 0; i < output.size(); i++) {
            result += output[i];
            if (i != output.size() - 1)
                result += ",";
        }

        return result;
    }

};

VM parse(const std::string& text) {
    std::regex pattern(R"(Register A: (\d+)\s*?\n?Register B: (\d+)\s*?\n?Register C: (\d+)\s*?\n?Program: ([\d,]+))");
    std::smatch matches;

    if (!std::regex_search(text, matches, pattern) || matches.size() != 5)
        throw std::runtime_error("invalid input format");

    auto vm = VM();
    vm.ra = std::stoul(matches[1]);
    vm.rb = std::stoul(matches[2]);
    vm.rc = std::stoul(matches[3]);

    std::string codestr = matches[4];
    vm.code = parse_ints(codestr, ",");
    
    return vm;
}

void selt_test() {
    assert(parse("Register A: 729\nRegister B: 0\nRegister C: 0\n\nProgram: 0,1,5,4,3,0").execute().output == "4635635210");
    assert(VM(2024, 0,        0, {0,1,5,4,3,0}).execute().output == "42567777310");
    assert(VM(10,   0,        0, {5,0,5,1,5,4}).execute().output == "012");
    assert(VM(0,    29,       0, {1,7}).execute().rb == 26);
    assert(VM(0,    2024, 43690, {4,0}).execute().rb == 44354);
}


int main() {
    selt_test();

    auto text = read_file_raw_text("input.txt");
    if (text.empty()) {
        std::cout << "unable to load input" << std::endl;
        return 0;
    }

    auto answer1 = parse(text).execute().output_csv();

    std::cout << "Answer 1: " << answer1 << std::endl;
}
