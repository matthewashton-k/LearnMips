/**
 * Description: header file for a static class that stores each of the validity functions for the challenges in each section of the Learn Mips app
 * Authors: Matthew Ashton Knochel, Carter Dean, Abdulla Alnuaimi, Logan Luker, Aidan Sheehan
*/

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
    try {
        unsigned int num = 3000;
        int n = 8; // Number of least significant bits to isolate

        unsigned int isolated = (num << (32 - n)) >> (32 - n);
        Interpreter interpreter2(code);
        interpreter2.run();
        return interpreter2.getV0() == isolated;
    } catch (string err) {
        cout << "ERROR: " + err << endl;
    }

}

bool ValidityFunctions::section3ValidityFunction(std::string code){
    try {
        Interpreter interpreter3(code);
        string expectedString = "Jerry STOLE 32767 eggs on his spree!";
        string outputString = interpreter3.run();
        return expectedString == outputString;

    } catch (string err) {
        cout << "ERROR: " + err << endl;
        return false;
    }
}

bool ValidityFunctions::section4ValidityFunction(std::string code){
    //5 + 5 + 5 + 5 + 5 + 5 + 6 + 7 + 8 + 9 + 10 + 11 + 12 = 93
    try {
        Interpreter interpreter4(code);
        interpreter4.run();
        cout << interpreter4.getV0() << endl;
        return interpreter4.getV0() == 93;
    } catch (string err) {
        cout << "ERROR: " + err << endl;
        return false;
    }
}

bool ValidityFunctions::section5ValidityFunction(std::string code){
    try {
        Interpreter interpreter5(code);
        interpreter5.run();
        return interpreter5.getV0() == 12;
    } catch (string err) {
        cout << "ERROR: " + err <<endl;
        return false;
    }
}

bool ValidityFunctions::section6ValidityFunction(std::string code){
    try {
        Interpreter interpreter6(code);
        interpreter6.run();
        // mystring should be HELLOWORLD
        optional<vector<uint8_t>> capitalized = interpreter6.getSymbol("myString", 10);
        if (!capitalized.has_value())
            throw ("No myString symbol found");
        string actual = "HELLOWORLD";
        for (int index = 0; index < 10; index++) {
            if (actual.at(index) != capitalized.value().at(index)) {
                return false;
            }
        }
    } catch (string err) {
        cout << "ERROR: " << err << endl;
        return false;
    }
    return true;
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
    try {
        Interpreter interpreter8(code);
        interpreter8.run();
        //$s1 contains 1532
        return (interpreter8.getV1() == 1542) && (interpreter8.getV0() == 5432);
    } catch (string err) {
        cout << "ERROR: " + err <<endl;
        return false;
    }
}

bool ValidityFunctions::section9ValidityFunction(std::string code){
    int tenthFibonacciValue = 55;
    try {
        Interpreter bonus1(code);
        bonus1.run();
        return bonus1.getV1() == tenthFibonacciValue;
    } catch (string err){
        cout << "ERROR: " << err << endl;
        return false;
    }
}

bool ValidityFunctions::section10ValidityFunction(std::string code){
    return false;
}

bool ValidityFunctions::section11ValidityFunction(std::string code){
    return false;
}
