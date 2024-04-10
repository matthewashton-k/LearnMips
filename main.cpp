#include "mainwindow.h"

#include <Interpreter.h>
#include <QApplication>
#include <iostream>
#include <string>
using std::cout;
using std::endl;

void testInterpreter();

int main(int argc, char *argv[])
{
    // testInterpreter();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}


void testInterpreter() {
    std::string instructions = R"(
        addi $t1, $zero, 2 # t1 = 2
        addi $t2, $zero, 3 # t2 = 3
        add $t3, $t1, $t2  # t3 = t1+t2
        sll $t3, $t3, 1    # t3 *= 2
        # test line comment
        sw $t3, 0($zero)   # stack[0] = t3
        addi $v0, $zero, 1 # v0 = 1 (print syscall code)
        lw $a0, 0($zero)   # a0 = stack[0]
        addi $t4, $zero, 5
        xori $a0, $a0, 5  # a0 = a0 XOR t4
        syscall            # should print out 15
    )";

    Interpreter interpreter(instructions);
    try {
        cout << interpreter.run() << endl;
    } catch (std::string err) {
        cout << "error";
        cout << err << endl;
    }

    std::string instructions_j_type = R"(
        firstlabel:#comment test
        addi $s0, $zero, 0
        addi $s1, $zero, 5
        label2: # do comments work
        addi $s0, $s0, 1       # s0 = 1
        bne $s0, $s1, label2   # if s0 != 5, continue loop
        addi $v0, $zero, 1     # print syscall
        add $a0, $zero, $s0   # print contents of s0 (5)
        syscall
    )";

    Interpreter interpreterJtype(instructions_j_type);
    try {
        cout << interpreterJtype.run() << endl;
    } catch (std::string err) {
        cout << "error";
        cout << err << endl;
    }
}
