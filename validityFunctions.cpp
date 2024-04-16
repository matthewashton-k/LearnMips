#include "validityFunctions.h"
#include "Interpreter.h"
#include <iostream>
using namespace std;

bool ValidityFunctions::section1ValidityFunction(std::string code) {
    try {
        Interpreter interpreter1(code);
        interpreter1.run();
        cout << interpreter1.getV0() << endl;
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

/// XOR challenge
bool ValidityFunctions::section7ValidityFunction(std::string code){
    try {
        // v0 should contain 11, the length of the ciphertext
        Interpreter interpreter7(code);
        interpreter7.run();
        if (interpreter7.getV0() != 11) {
            return false;
        }
        optional<vector<uint8_t>> ciphertext = interpreter7.getSymbol("ciphertext", interpreter7.getV0());
        if (!ciphertext.has_value()) {
            return false;
        }

        uint8_t correctAns[11] = {0x04, 0x19, 0x00, 0x1c, 0x17, 0x0b, 0x0c, 0x19, 0x1b, 0x15, 0x05};
        for (int i = 0; i < 11; i++) {
            if (ciphertext.value().at(i) != correctAns[i]) {
                return false;
            }
        }
    } catch (string err) {
        cout << "ERROR: " + err << endl;
    }

    return true;
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
