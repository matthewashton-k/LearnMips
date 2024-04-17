#include "challenge.h"
#include <string>
#include <iostream>
#include "Interpreter.h"

Challenge::Challenge() {}

Challenge::Challenge(std::string beforeCode, std::string afterCode, bool (*validityFunction)(std::string instructions)) :
    beforeCode(beforeCode),
    afterCode(afterCode),
    validityFunction(validityFunction) {}

std::tuple<std::string, bool> Challenge::executeCode(std::string middleCode, bool checkSolutionValidity){
    //run code
    std::string instructions = beforeCode + "\n" + middleCode + "\n" + afterCode;

    std::string outputString;
    bool isValidSolution;

    //if checkSolutionValidy, run the relevant challenges checkChallenge function, otherwise, just run the code
    if(checkSolutionValidity){
        isValidSolution = validityFunction(instructions);
        if(isValidSolution) outputString = "Challenge Cleared!";
        else outputString = "Invalid Solution";
    }
    else{
        Interpreter interpreter(instructions);
        isValidSolution = true;

        try {
            outputString = interpreter.run();
        } catch (std::string err) {
            outputString = err;
            std::cout << "Error: " << err << std::endl;
        }
    }

    return std::make_tuple(outputString, isValidSolution);
}
