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
        line = trimWhitespace(line);
        if (line.empty()) {
            continue;
        }
        instructions.push_back(line); // also trim the whitespace
    }
    findLabels();
}

void Interpreter::addi(Reg dst, Reg src, int val) {
    registers[dst] = registers[src] + val;
}

void Interpreter::subi(Reg dst, Reg src, int inVal){
    registers[dst] = registers[src] - inVal;
}

void Interpreter::xori(Reg dst, Reg src, int inVal) {
    registers[dst] = registers[src] ^ inVal;
}

void Interpreter::Xor(Reg dst, Reg src1, Reg src2) {
   registers[dst] = registers[src1] xor registers[src2];
}

void Interpreter::add(Reg dst, Reg src1, Reg src2) {
    registers[dst] = registers[src1] + registers[src2];
}

void Interpreter::sub(Reg dst, Reg src1, Reg src2) {
    registers[dst] = registers[src1] - registers[src2];
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

void Interpreter::beq(Reg src1, Reg src2, int offset) {
    if (registers[src1]==registers[src2])
        this->programCounter = offset;
}

void Interpreter::blt(Reg src1, Reg src2, int offset) {
    if (registers[src1]<registers[src2])
        this->programCounter = offset;
}

void Interpreter::bgt(Reg src1, Reg src2, int offset) {
    if (registers[src1]>registers[src2])
        this->programCounter = offset;
}

void Interpreter::bne(Reg src1, Reg src2, int offset) {
    if (registers[src1] != registers[src2])
        this->programCounter = offset;
}

void Interpreter::j(int offset) {
    this->programCounter = offset;
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


void Interpreter::findLabels() {
    int index = 0;
    for (auto line : instructions) {
        if (isLabel(line)) {
            this->labels.insert({line.substr(0, line.find(":")), index});
        }
        index++;
    }
}

bool Interpreter::isLabel(std::string instruction) {
    // If '#' is found, remove everything after it (including '#')
    size_t commentPos = instruction.find('#');
    if (commentPos != std::string::npos) {
        instruction = instruction.substr(0, commentPos);
    }
    // trim whitespace
    instruction = trimWhitespace(instruction);
    // Check if the string is empty
    if (instruction.empty()) {
        return false;
    }

    // Find the position of the colon ':'
    size_t colonPos = instruction.find(':');

    // If no colon is found or it is the first char, it's not a valid label
    if (colonPos == std::string::npos || colonPos == 0) {
        return false;
    }

    // check if the characters before the colon are valid label characters
    for (size_t i = 0; i < colonPos; ++i) {
        char c = instruction[i];
        if (!std::isalnum(c) && c != '_') {
            return false;
        }
    }

    // If all conditions are satisfied, it is a valid MIPS label
    return true;
}


static const std::unordered_map<std::string, Opcode> opcodeMap = {
    {"addi", Opcode::addi},
    {"add", Opcode::add},
    {"xori", Opcode::xori},
    {"sll", Opcode::sll},
    {"sub", Opcode::sub},
    {"xor", Opcode::Xor},
    {"srl", Opcode::srl},
    {"lw", Opcode::lw},
    {"sw",Opcode::sw},
    {"beq", Opcode::beq},
    {"bne", Opcode::bne},
    {"j", Opcode::j},
    {"blt", Opcode::blt},
    {"bgt", Opcode::bgt},
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

std::string Interpreter::trimWhitespace(std::string& str) {
    size_t first = str.find_first_not_of(" \t");
    if (std::string::npos == first) {
        return "";
    }
    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, (last - first + 1));
}

vector<string> Interpreter::tokenize(const string& str, char delimiter) {
    vector<string> tokens;
    stringstream ss(str);
    std::string token;

    while (getline(ss, token, delimiter)) {
        // remove comma from the token

        auto it = std::find(token.begin(), token.end(), ',');
        if (it != token.end())
            token.erase(it);

        size_t commentPos = token.find("#");
        if (commentPos != std::string::npos) {
            break;
        }
        token = trimWhitespace(token);
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
    int jumps = 0;
    while (programCounter < instructions.size()) {
        auto instruction = instructions.at(programCounter);
        this->programCounter++;
        if (instruction.empty() || isLabel(instruction)) {
            continue;
        }

        vector<string> tokens = tokenize(instruction, ' ');
        if (tokens.empty()) {
            continue;
        }

        std::optional<Opcode> opcode = parseOpcode(tokens[0]);
        if (!opcode.has_value()) {
            throw ("No opcode near instruction: " + instruction);
        }

        std::optional<Reg> dstReg = (tokens.size() > 2) ? parseReg(tokens[1]) : nullopt;
        if (!dstReg.has_value() && opcode.value() != Opcode::syscall) {
            throw ("No destination register near instruction: " + instruction);
        }

        switch (opcode.value()) {
            case Opcode::srl:
            case Opcode::sll:
            case Opcode::addi:
            case Opcode::subi:
            case Opcode::xori:
                executeImmediateInstruction(opcode.value(), dstReg.value(), tokens, instruction);
                break;
            case Opcode::add:
            case Opcode::sub:
            case Opcode::Xor:
                executeRegisterInstruction(opcode.value(), dstReg.value(), tokens, instruction);
                break;
            case Opcode::lw:
            case Opcode::sw:
                executeMemoryInstruction(opcode.value(), dstReg.value(), tokens, instruction);
                break;
            case Opcode::beq:
            case Opcode::bne:
            case Opcode::bgt:
            case Opcode::blt:
            case Opcode::j:
                // TODO: j doesnt work yet
                if (jumps == maxJumps){
                    throw (string("Max number of jumps exceeded: aborting."));
                }
                executeJumpInstruction(opcode.value(), tokens,instruction);
                jumps++;
                break;
            case Opcode::syscall:
                syscall(static_cast<Syscall>(registers[Reg::v0]));
                break;

            default:
                // Should never reach here
                break;
        }
    }
    return this->stdOut;
}

void Interpreter::executeImmediateInstruction(Opcode opcode, Reg dstReg, const vector<string>& tokens, const string& instruction) {
    std::optional<Reg> srcReg1 = parseReg(tokens[2]);
    int imm;
    try {
        imm = stoi(tokens[3]);
    } catch (exception e){
        throw ("No immediate value at: " + instruction);
    }

    if (!srcReg1.has_value()) {
        throw ("Invalid source registebner at: " + instruction);
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
        case Opcode::subi: {
            subi(dstReg, srcReg1.value(), imm);
            break;
        }
        case Opcode::xori: {
            xori(dstReg, srcReg1.value(), imm);
            break;
        }
        default:
            throw ("Instruction is not I-type at: "+ instruction);
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
        case Opcode::Xor: {
            Xor(dstReg, srcReg1.value(), srcReg2.value());
            break;
        }
        default:
            throw ("Instruction is not R-type at: "+ instruction);
    }
}

void Interpreter::executeMemoryInstruction(Opcode opcode, Reg dstReg, const vector<string>& tokens, const string& instruction) {
    std::optional<Reg> srcReg;
    int offset = 0;

    // Extract the offset and source/destination register
    size_t openParenIndex = tokens[2].find_first_of('(');
    if (openParenIndex != std::string::npos) {
        try {
            offset = std::stoi(tokens[2].substr(0, openParenIndex));
            size_t closeParenIndex = tokens[2].find_last_of(')');
            if (closeParenIndex != std::string::npos) {
                std::string innerRegStr = tokens[2].substr(openParenIndex + 1, closeParenIndex - openParenIndex - 1);
                srcReg = parseReg(innerRegStr);
            }
        } catch (exception e) { // if stoi fails
            throw ("Invalid instruction format at: " + instruction);
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


void Interpreter::executeJumpInstruction(Opcode opcode, const std::vector<string> &tokens, const string &instruction) {
    string label = tokens.back();
    auto it = labels.find(label);
    int offset;
    if (it != labels.end()) {
        offset = it->second;
    } else {
        throw ("Invalid instruction at: " + instruction);
    }
    std::optional<Reg> srcReg1 = parseReg(tokens[1]);
    std::optional<Reg> srcReg2 = parseReg(tokens[2]);
    if (!srcReg1.has_value()) {
        throw ("Invalid source register at: " + instruction);
    }
    if (!srcReg2.has_value()) {
        throw ("Invalid source register at: " + instruction);
    }
    // TODO: handle j opcode
    switch (opcode) {
        case Opcode::beq: {
            beq(srcReg1.value(), srcReg2.value(), offset);
            break;
        }
        case Opcode::bne: {
            bne(srcReg1.value(), srcReg2.value(), offset);
            break;
        }
        case Opcode::blt: {
            blt(srcReg1.value(), srcReg2.value(), offset);
            break;
        }
        case Opcode::bgt: {
            bgt(srcReg1.value(), srcReg2.value(), offset);
            break;
        }
        default: {
            cout << "right here";
            throw ("Invalid opcode at " + instruction);
        }
    }
}
