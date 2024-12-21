#include "../common/util.h"

struct VM {
    uint32_t ra, rb, rc;
    std::vector<char> code;
    std::vector<char> output;
    int ip;

    VM() {}

    VM(const uint32_t ra, const uint32_t rb, const uint32_t rc, const std::vector<char> code)
        : code(code)
        , ra(ra), rb(rb), rc(rc)
        , ip(0) {}

    VM(const std::string& input) {
        std::regex pattern(R"(Register A: (\d+)\s*?\n?Register B: (\d+)\s*?\n?Register C: (\d+)\s*?\n?(?:Program:\s*([\d,]+)))");
        std::smatch matches;

        if (!std::regex_search(input, matches, pattern) || matches.size() != 5)
            throw std::runtime_error("invalid input format");

        ra = std::stoul(matches[1]);
        rb = std::stoul(matches[2]);
        rc = std::stoul(matches[3]);

        for (auto& c : matches[4].str())
            if (std::isdigit(c))
                code.push_back(c - '0');
    }

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

    std::string disasm_combo(char operand) {
        switch (operand) {
            case 0:
            case 1:
            case 2:
            case 3: return std::to_string(operand & 3);
            case 4: return "ra";
            case 5: return "rb";
            case 6: return "rc";
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
                output.push_back(combo(operand) % 8);
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

    std::string disasm(int opcode, int operand) {
        switch (opcode) {
            case OpcodeAdv:
                return "ra >>= " + disasm_combo(operand);
            case OpcodeBxl:
                return "rb ^= " + std::to_string(operand);
            case OpcodeBst:
                return "rb = " + disasm_combo(operand);
            case OpcodeJnz:
                return "if (ra) goto " + std::to_string(operand);
            case OpcodeBxc:
                return "rb ^= rc";
            case OpcodeOut:
                return "out " + disasm_combo(operand) + " & 7";
            case OpcodeBdv:
                return "rb = ra >> " + disasm_combo(operand);
            case OpcodeCdv:
                return "rc = ra >> " + disasm_combo(operand);
            default: throw std::runtime_error("Unknown opcode");
        }
    }

    std::string disasm() {
        std::stringstream ss;
        ss << "Registers:" << "\n";
        ss << " ra: " << ra << "\n";
        ss << " rb: " << rb << "\n";
        ss << " rc: " << rc << "\n";
        ss << "----------" << "\n";
        ss << "Code: " << code_str() << std::endl;
        ss << "----------" << "\n";
        ss << "Dissassembly:" << "\n";

        for (int i = 0; i < code.size(); i += 2) {
            auto opcode  = code[i];
            auto operand = code[i+1];

            ss << "0x" << std::setw(2) << std::setfill('0') << i << "    ";

            ss << disasm(opcode, operand) << "\n";
        }
        return ss.str();
    }

    VM& execute(int start = 0) {
        ip = start;
        while (ip < code.size())
            single_step();

        return *this;
    }

    uint32_t bruteforce_quine() {
        for (uint64_t i = 0; i < UINT32_MAX; i++) {
            ra = i;
            rb = 0;
            rc = 0;

            output.clear();
            ip = 0;

            if (i % 0x100000 == 0)
                std::cout << "Answer Part 2: Bruteforcing...\r" << std::flush;

            int cycles = 0;
            while (ip < code.size() && output.size() <= code.size()) {
                single_step();
            }

            if (code == output)
                return i;
        }
        return 0;
    }

    std::string code_str() {
        return stringify(code);
    }

    std::string output_str() {
        return stringify(output);
    }

    std::string stringify(const std::vector<char>& v) {
        auto s = std::string{};

        for (auto& c : v) {
            s.push_back('0'+c);
            s.push_back(',');
        }
        
        if (!s.empty())
            s.pop_back();

        return s;
    }
};

void selt_test() {
    assert(VM("Register A: 729\nRegister B: 0\nRegister C: 0\n\nProgram: 0,1,5,4,3,0").execute().output_str() == "4,6,3,5,6,3,5,2,1,0");
    assert(VM(2024, 0,        0, {0,1,5,4,3,0}).execute().output_str() == "4,2,5,6,7,7,7,7,3,1,0");
    assert(VM(10,   0,        0, {5,0,5,1,5,4}).execute().output_str() == "0,1,2");
    assert(VM(0,    29,       0, {1,7}).execute().rb == 26);
    assert(VM(0,    2024, 43690, {4,0}).execute().rb == 44354);
}


int main() {
    //selt_test();

    auto text = read_file_raw_text("input.txt");
    if (text.empty()) {
        std::cout << "unable to load input" << std::endl;
        return 0;
    }

    auto vm = VM(text);
    std::cout << vm.disasm() << std::endl << std::endl;

    auto answer1 = vm.execute().output_str();
    std::cout << "Answer Part 1: " << answer1 << std::endl;

    //auto answer2 = vm.bruteforce_quine();
    //std::cout << "Answer Part 2: " << answer2 << std::endl;

}
