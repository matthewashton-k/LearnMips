#include "Interpreter.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <optional>
#include <unordered_map>
#include <algorithm>

using namespace std;
Interpreter::Interpreter(std::string instructionsStr) {
    std::istringstream iss(instructionsStr);
    std::string line;
    while (std::getline(iss, line)) {
        instructions.push_back(line);
    }
}

void Interpreter::addi(Reg dst, Reg src, int val) {
    registers[dst] = registers[src] + val;
}

void Interpreter::add(Reg dst, Reg src1, Reg src2) {
    registers[dst] = registers[src1] + registers[src2];
}

void Interpreter::sub(Reg dst, Reg src1, Reg src2) {
    registers[dst] = registers[src1] - registers[src2];
}

void Interpreter::mul(Reg dst, Reg src1, Reg src2) {
    registers[dst] = registers[src1] * registers[src2];
}

void Interpreter::sll(Reg dst, Reg src, int amount) {
    registers[dst] = registers[src] << amount;
}

void Interpreter::srl(Reg dst, Reg src, int amount) {
    registers[dst] = registers[src] >> amount;
}

void Interpreter::sw(Reg src, Reg src2, int offset) {
    stack[registers[src2] + offset] = registers[src];
}

void Interpreter::lw(Reg dst, Reg src, int offset) {
    registers[dst] = stack[registers[src] + offset];
}

void Interpreter::syscall(Syscall code) {
    switch (code) {
        case printInteger:
            std::cout << registers[a0] << std::endl;
            break;
        case Syscall::Exit:
            exit(registers[a0]);
            break;
    }
}

void Interpreter::reset() {
    std::fill_n(this->registers,13, 0);
    std::fill_n(this->stack, 32, 0);
}

static const std::unordered_map<std::string, Opcode> opcodeMap = {
        {"addi", Opcode::addi},
        {"add", Opcode::add},
        {"sll", Opcode::sll},
        {"sub", Opcode::sub},
        {"mul", Opcode::mul},
        {"srl", Opcode::srl},
        {"lw", Opcode::lw},
        {"sw",Opcode::sw},
        {"syscall", Opcode::syscall}
};
static const std::unordered_map<std::string, Reg> regMap = {
        {"$v0", v0},
        {"$v1", v1},
        {"$a0", a0},
        {"$s0", s0},
        {"$s1", s1},
        {"$s2", s2},
        {"$s3", s3},
        {"$s4", s4},
        {"$t1", t1},
        {"$t2", t2},
        {"$t3", t3},
        {"$t4", t4},
        {"$zero", zero}
};

std::optional<Opcode> parseOpcode(const std::string& opcodeStr) {
    auto it = opcodeMap.find(opcodeStr);
    if (it != opcodeMap.end()) {
        return it->second;
    }
    return std::nullopt;
}

optional<Reg> parseReg(const std::string& regStr) {
    const auto it = regMap.find(regStr);
    if (it != regMap.end()) {
        return it->second;
    }

    return nullopt; // Return nullopt if the register string is invalid
}
vector<string> tokenize(const string& str, char delimiter) {
    vector<string> tokens;
    stringstream ss(str);
    string token;

    while (getline(ss, token, delimiter)) {
        auto it = std::find(token.begin(), token.end(), ',');
        if (it != token.end())
            token.erase(it);
        if (token.empty()) {
            continue;
        }
        tokens.push_back(token);
    }

    return tokens;
}

/**
 * @brief returns a string with stdout or throws a string as a compile time/run time error
 * @return
 */
std::string Interpreter::run() {
    // Check for syntax errors
    // ...

    // Interpret instructions
    for (const auto& instruction : instructions) {
        if (instruction.empty()) continue;

        vector<string> tokens = tokenize(instruction, ' ');
        if (tokens.empty()) continue;

        std::optional<Opcode> opcode = parseOpcode(tokens[0]);
        if (!opcode.has_value()) {
            throw ("no opcode near instruction: " + instruction);
        }

        std::optional<Reg> dstReg = (tokens.size() > 2) ? parseReg(tokens[1]) : nullopt;
        if (!dstReg.has_value() && opcode.value() != Opcode::syscall) {
            throw ("No destination register near instruction: " + instruction);
        }

        switch (opcode.value()) {
            case Opcode::srl:
            case Opcode::sll:
            case Opcode::addi:
                executeImmediateInstruction(opcode.value(), dstReg.value(), tokens, instruction);
                break;
            case Opcode::add:
            case Opcode::sub:
            case Opcode::mul:
                executeRegisterInstruction(opcode.value(), dstReg.value(), tokens, instruction);
                break;
            case Opcode::lw:
            case Opcode::sw:
                executeMemoryInstruction(opcode.value(), dstReg.value(), tokens, instruction);
                break;

            case Opcode::syscall:
                syscall(static_cast<Syscall>(registers[Reg::v0]));
                break;

            default:
                // Should never reach here
                break;
        }
    }

    return this->stdout;
}

void Interpreter::executeImmediateInstruction(Opcode opcode, Reg dstReg, const vector<string>& tokens, const string& instruction) {
    std::optional<Reg> srcReg1 = parseReg(tokens[2]);
    int imm = stoi(tokens[3]);
    if (!srcReg1.has_value()) {
        throw ("Invalid source register at: " + instruction);
    }
    switch (opcode) {
        case Opcode::addi: {
            addi(dstReg, srcReg1.value(), imm);
            break;
        }
        case Opcode::sll: {
            sll(dstReg, srcReg1.value(), imm);
            break;
        }
        case Opcode::srl: {
            srl(dstReg, srcReg1.value(), imm);
            break;
        }
    }
}

void Interpreter::executeRegisterInstruction(Opcode opcode, Reg dstReg, const vector<string>& tokens, const string& instruction) {
    std::optional<Reg> srcReg1 = parseReg(tokens[2]);
    std::optional<Reg> srcReg2 = parseReg(tokens[3]);
    if (!srcReg1.has_value()) {
        throw ("Invalid source register at: " + instruction);
    }
    if (!srcReg2.has_value()) {
        throw ("Invalid source register at: " + instruction);
    }
    switch (opcode) {
        case Opcode::add: {
            add(dstReg, srcReg1.value(), srcReg2.value());
            break;
        }
        case Opcode::sub: {
            sub(dstReg, srcReg1.value(), srcReg2.value());
            break;
        }
        case Opcode::mul: {
            mul(dstReg, srcReg1.value(), srcReg2.value());
            break;
        }
        default:
            throw ("Instruction is not I-type at: "+ instruction);
    }
}

void Interpreter::executeMemoryInstruction(Opcode opcode, Reg dstReg, const vector<string>& tokens, const string& instruction) {
    std::optional<Reg> srcReg;
    int offset = 0;

    // Extract the offset and source/destination register
    size_t openParenIndex = tokens[2].find_first_of('(');
    if (openParenIndex != std::string::npos) {
        offset = std::stoi(tokens[2].substr(0, openParenIndex));
        size_t closeParenIndex = tokens[2].find_last_of(')');
        if (closeParenIndex != std::string::npos) {
            std::string innerRegStr = tokens[2].substr(openParenIndex + 1, closeParenIndex - openParenIndex - 1);
            srcReg = parseReg(innerRegStr);
        }
    } else {
        throw ("Invalid instruction format at: " + instruction);
    }

    if (!srcReg.has_value()) {
        throw ("Invalid source/destination register at: " + instruction);
    }

    switch (opcode) {
        case Opcode::lw:
            lw(dstReg, srcReg.value(), offset);
            break;

        case Opcode::sw:
            std::optional<Reg> srcRegister = parseReg(tokens[1]);
            if (!srcRegister.has_value()) {
                throw ("Invalid source register at: " + instruction);
            }
            sw(srcRegister.value(), srcReg.value(), offset);
            break;
    }
}