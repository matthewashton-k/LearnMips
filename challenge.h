#ifndef CHALLENGE_H
#define CHALLENGE_H

#include <string>

class Challenge
{
private:
    std::string beforeCode = "";
    std::string afterCode = "";
    //bool checkChallenge(std::string middleCode);
    //pointer to validity function
    bool (*checkChallenge)(std::string) = [](std::string){return true;};
public:
    Challenge();
    Challenge(std::string beforeCode, std::string afterCode, bool (*checkChallenge)(std::string middleCode));

};

#endif // CHALLENGE_H
