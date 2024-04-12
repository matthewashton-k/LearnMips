#include "section.h"

Section::Section() : challenge(Challenge()){}

Section::Section(Challenge challenge) : challenge(challenge) {}

Challenge* Section::getChallenge(){
    return &challenge;
}
