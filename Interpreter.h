//
// Created by thivmcthiv on 4/2/24.
//

#ifndef MIPS_INTERPRETER_INTERPRETER_H
#define MIPS_INTERPRETER_INTERPRETER_H
#include <vector>
#include <string>
#include <variant>
#include <optional>
#include <unordered_map>


/**
 * @brief the value for each variant is its index in the register Array
 */
enum Reg {
    v0  ,
    v1  ,
    a0  ,
    s0  ,
    s1  ,
    s2  ,
    s3  ,
    s4  ,
    s5  ,
    s6  ,
    t0  ,
    t1  ,
    t2  ,
    t3  ,
    t4  ,
    t5  ,
    t6  ,
    zero ,
    sp  ,
};

/**
 * @brief The Syscall enum
 */
enum Syscall {
    printInteger = 1,
    Exit = 10,
    printString = 4,
};

/**
 * @brief The Opcode enum
 */
enum Opcode {
    addi,
    subi,
    xori,
    // yes this is capital, xor is a reserved keyword
    Xor,
    add,
    sub,
    sll,
    srl,
    lw,
    sw,
    lb,
    sb,
    la,
    beq,
    bne,
    j,
    blt,
    bgt,
    syscall
};

enum CodeSection{
    Data,
    Text,
    Default
};


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
    {"lb", Opcode::lb},
    {"sb",Opcode::sb},
    {"la",Opcode::la},
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
    {"$s5", s5},
    {"$s6", s6},
    {"$t0", t0},
    {"$t1", t1},
    {"$t2", t2},
    {"$t3", t3},
    {"$t4", t4},
    {"$t5", t5},
    {"$t6", t6},
    {"$zero", zero},
    {"$sp", sp}
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
     * @brief maxJumps used to set a max number of times a jump can occur, this is to prevent infinite loops
     */
    int maxJumps = 1000;


    /**
     * @brief registers holds the contents of the registers, ex: registers[Opcode::v0]
     *
     */
    int registers[19] = {0};

    /**
     * @brief programCounter the index in the vector of instructions that is currently being executed
     */
    int programCounter = 0;

    /**
     * @brief stack holds the
     */
    // uint8_t stack[128] = {0};
    std::vector<uint8_t> stack;
    std::vector<std::string> instructions;

    /**
     * @brief labels contains the labels in the .text section only. <label, offset>
     */
    std::unordered_map<std::string, int> labels;

    /**
     * @brief dataLabels contains labels in the data section only, <label, offset in stack>
     */
    std::unordered_map<std::string, int> dataLabels;
    /**
     * @brief stdout holds the standard output of the interpreter if someone has a syscall to print
     */
    std::string stdOut = "";

    void addi(Reg dst, Reg src, int inVal);
    void subi(Reg dst, Reg src, int inVal);
    void xori(Reg dst, Reg src, int inVal);
    // yes its capital, xor is a reserved keyword
    void Xor(Reg dst, Reg src1, Reg src2);
    void add(Reg dst, Reg src1, Reg src2);
    void sub(Reg dst, Reg src1, Reg src2);
    void sll(Reg dst, Reg src, int amount);
    void srl(Reg dst, Reg src, int amount);
    void sw(Reg src, Reg src2, int offset);
    void lw(Reg dst, Reg src1, int src2);
    void sb(Reg src, Reg src2, int offset);
    void lb(Reg dst, Reg src1, int src2);
    void la(Reg dst, int offset);
    void beq(Reg src1, Reg src2, int offset);
    void bne(Reg src1, Reg src2, int offset);
    void bgt(Reg src1, Reg src2, int offset);
    void blt(Reg src1, Reg src2, int offset);
    void j(int offset);
    void syscall(Syscall code);

    /**
     * @brief executeImmediateInstruction used to execute any instruction that ends with i (addi subi, etc), and srl, sll
     * @param opcode the type of opcode
     * @param dstReg the register where the resulting operation will be stored
     * @param tokens the tokens in the instruction as a vector e.g. [addi, $s0, $s1, 4]
     * @param instruction the tokens as a complete string. (may have preceding spaces)
     * @throws a string describing a syntax error
     */
    void executeImmediateInstruction(Opcode opcode, Reg dstReg, const std::vector<std::string>& tokens, const std::string& instruction);

    /**
     * @brief executeMemoryInstruction in charge of executing lw,sw, and anything that touches the stack
     * @param opcode the type of memory instruction
     * @param dstReg for load word, this is the register where the stuff from memory will be stored
     * @param tokens the tokens in the instruction as a vector
     * @param instruction the instrution being executed
     * @throws a string describing a syntax error
     */
    void executeMemoryInstruction(Opcode opcode, Reg dstReg, const std::vector<std::string>& tokens, const std::string& instruction);

    /**
     * @brief executeRegisterInstruction in charge of executing instructions with one dst register, and two src registers, e.g. mul, add, sub
     * @param opcode the type of r instruction
     * @param dstReg the register being written to
     * @param tokens a vector of tokens in the instruction
     * @param instruction the instruction as a string
     * @throws a string describing a syntax error
     */
    void executeRegisterInstruction(Opcode opcode, Reg dstReg, const std::vector<std::string>& tokens, const std::string& instruction);

    void executeJumpInstruction(Opcode opcode, const std::vector<std::string>& tokens, const std::string& instruction);

    /**
     * @brief findLabels helper function called in the constructor that finds labels and stores them in a hashmap of <string, int> where int is the offset in the stack
     */
    void findLabels();

    /**
     * @brief isLabel decides if an instruction is a label or not
     * @param instruction the line that may or may not be a label
     * @return true if the instruction is a label
     */
    bool isLabel(std::string instruction);

    /**
     * @brief trimWhitespace removes whitespace from the beginning and end of a string (does not mutate str)
     * @param str the string to remove whitespace from
     * @return the trimmed string
     */
    std::string trimWhitespace(std::string& str);

    /**
     * @brief tokenize tokenizes an instruction, trimming whitespace, removing commas, and removing comments
     * @param str the string to tokenize
     * @param delimiter
     * @return returns a vector of tokens
     */
    std::vector<std::string> tokenize(const std::string& str, char delimiter);

    /**
     * @brief isStringLabel decides if an instruction is a label of a string or not
     * @param instruction the line that may or may not be a label
     * @return true if the instruction is a label of a string
     */
    bool isStringLabel(std::string instruction);

    /**
     * @brief addStringLabel adds the string label to labels map
     * @param instruction the line that may or may not be a label
     */
    void addStringLabel(std::string instruction);



};


#endif //MIPS_INTERPRETER_INTERPRETER_H
