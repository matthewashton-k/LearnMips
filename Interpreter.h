//
// Created by thivmcthiv on 4/2/24.
//

#ifndef MIPS_INTERPRETER_INTERPRETER_H
#define MIPS_INTERPRETER_INTERPRETER_H
#include <vector>
#include <string>
#include <variant>
#include <optional>
/**
 * @brief the value for each variant is its index in the register Array
 */
enum Reg {
    v0 = 0,
    v1 = 1,
    a0 = 2,
    s0 = 3,
    s1 = 4,
    s2 = 5,
    s3 = 6,
    s4 = 7,
    t1 = 8,
    t2 = 9,
    t3 = 10,
    t4 = 11,
    zero = 12,
};

/**
 * @brief The Syscall enum
 */
enum Syscall {
    printInteger = 1,
    Exit = 10,
};

/**
 * @brief The Opcode enum
 */
enum Opcode {
    addi,
    add,
    sub,
    mul,
    sll,
    srl,
    lw,
    sw,
    syscall
};

class Interpreter {
public:
    /**
     * @brief reads in all the instructions, then goes line by line and stores them in the instructions vector
     * @param instructions
     */
    Interpreter(std::string instructions);

    /**
     * @brief loops through each of the instructions to check for syntax errors, then another loop is started for interpreting the instructions.
     * @return
     */
    std::string run();
    void reset();
    int getV0() {
        return registers[Reg::v0];
    }

private:
    /**
     * @brief registers holds the contents of the registers, ex: registers[Opcode::v0]
     *
     */
    int registers[13] = {0};

    /**
     * @brief stack holds the
     */
    int stack[32] = {0};
    std::vector<std::string> instructions;

    /**
     * @brief stdout holds the standard output of the interpreter if someone has a syscall to print
     */
    std::string stdout = "";
    void addi(Reg dst, Reg src, int val);
    void add(Reg dst, Reg src1, Reg src2);
    void sub(Reg dst, Reg src1, Reg src2);
    void mul(Reg dst, Reg src1, Reg src2);
    void sll(Reg dst, Reg src, int amount);
    void srl(Reg dst, Reg src, int amount);
    void sw(Reg src, Reg src2, int offset);
    void lw(Reg dst, Reg src1, int src2);
    void syscall(Syscall code);

    /**
     * @brief executeImmediateInstruction used to execute any instruction that ends with i (addi subi, etc), and srl, sll
     * @param opcode the type of opcode
     * @param dstReg the register where the resulting operation will be stored
     * @param tokens the tokens in the instruction as a vector e.g. [addi, $s0, $s1, 4]
     * @param instruction the tokens as a complete string. (may have preceding spaces)
     */
    void executeImmediateInstruction(Opcode opcode, Reg dstReg, const std::vector<std::string>& tokens, const std::string& instruction);

    /**
     * @brief executeMemoryInstruction in charge of executing lw,sw, and anything that touches the stack
     * @param opcode the type of memory instruction
     * @param dstReg for load word, this is the register where the stuff from memory will be stored
     * @param tokens
     * @param instruction
     */
    void executeMemoryInstruction(Opcode opcode, Reg dstReg, const std::vector<std::string>& tokens, const std::string& instruction);
    void executeRegisterInstruction(Opcode opcode, Reg dstReg, const std::vector<std::string>& tokens, const std::string& instruction);
};


#endif //MIPS_INTERPRETER_INTERPRETER_H
