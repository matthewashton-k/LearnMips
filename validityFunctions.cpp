#include "validityFunctions.h"
#include "Interpreter.h"
#include <iostream>
using namespace std;

bool ValidityFunctions::section1ValidityFunction(std::string code) {
    try {
        Interpreter interpreter1(code);
        interpreter1.run();
        return interpreter1.getV0() == 14;
    } catch (string err) {
        cout << "ERROR: " + err << endl;
        return false;
    }
}

bool ValidityFunctions::section2ValidityFunction(std::string code){
    return false;
}

bool ValidityFunctions::section3ValidityFunction(std::string code){
    return false;
}

bool ValidityFunctions::section4ValidityFunction(std::string code){
    return false;
}

bool ValidityFunctions::section5ValidityFunction(std::string code){
    return false;
}

bool ValidityFunctions::section6ValidityFunction(std::string code){
    return false;
}

bool ValidityFunctions::section7ValidityFunction(std::string code){
    return false;
}

bool ValidityFunctions::section8ValidityFunction(std::string code){
    return false;
}

bool ValidityFunctions::section9ValidityFunction(std::string code){
    return false;
}

bool ValidityFunctions::section10ValidityFunction(std::string code){
    return false;
}

bool ValidityFunctions::section11ValidityFunction(std::string code){
    return false;
}
