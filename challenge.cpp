#include "challenge.h"
#include <string>

Challenge::Challenge() {}

Challenge::Challenge(std::string beforeCode, std::string afterCode, bool (*validityFunction)(std::string middleCode)) :
    beforeCode(beforeCode),
    afterCode(afterCode),
    validityFunction(validityFunction) {}

bool Challenge::checkChallenge(std::string middleCode){
    //create full code string
    std::string fullCode = beforeCode + middleCode + afterCode;

    //check against validity function
    return validityFunction(fullCode);
}
