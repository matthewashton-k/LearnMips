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
    testInterpreter();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}


void testInterpreter() {
    std::string instructions = R"(
        addi $t1, $zero, 2
        addi $t2, $zero, 3
        add $t3, $t1, $t2
        sll $t3, $t3, 1
        sw $t3, 0($zero)
        addi $v0, $zero, 1
        lw $t4, 0($zero)
        mul $a0, $t4, $t4
        syscall
    )";

    Interpreter interpreter(instructions);
    try {
        cout << interpreter.run() << endl;
    } catch (std::string err) {
        cout << "error";
        cout << err << endl;
    }
}
