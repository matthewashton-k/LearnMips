#ifndef CHALLENGE_H
#define CHALLENGE_H

#include <string>

class Challenge
{
private:
    std::string beforeCode = "";
    std::string afterCode = "";
    //bool validityFunction(std::string middleCode);
    //pointer to validity function
    bool (*validityFunction)(std::string) = [](std::string){return true;};
public:
    Challenge();
    Challenge(std::string beforeCode, std::string afterCode, bool (*validityFunction)(std::string middleCode));

    std::tuple<std::string, bool> executeCode(std::string middleCode, bool checkSolutionValidity);
};

#endif // CHALLENGE_H
