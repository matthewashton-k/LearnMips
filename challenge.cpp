#include "challenge.h"
#include <string>

Challenge::Challenge() {}

Challenge::Challenge(std::string beforeCode, std::string afterCode, bool (*checkChallenge)(std::string middleCode)) :
    beforeCode(beforeCode),
    afterCode(afterCode),
    checkChallenge(checkChallenge) {}
