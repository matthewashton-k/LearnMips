#include "section.h"

Section::Section() : challenge(Challenge()){}

Section::Section(Challenge challenge) : challenge(challenge) {}

const Challenge* Section::getChallenge(){
    return &challenge;
}
