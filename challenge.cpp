/**
 * Description: cpp file for the challenge class, which represents a coding challenge with an ability to check if a solution is valid
 * Authors: Matthew Ashton Knochel, Carter Dean, Abdulla Alnuaimi, Logan Luker, Aidan Sheehan
*/

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
    //combine code
    std::string instructions = beforeCode + "\n" + middleCode + "\n" + afterCode;

    //declare output variables
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

    //return tuple
    return std::make_tuple(outputString, isValidSolution);
}
