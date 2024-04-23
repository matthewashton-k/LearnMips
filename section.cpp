/**
 * Description: cpp file for a class representing a section in the Learn Mips app
 * Authors: Matthew Ashton Knochel, Carter Dean, Abdulla Alnuaimi, Logan Luker, Aidan Sheehan
*/
#include "section.h"

Section::Section() : challenge(Challenge()){}

Section::Section(Challenge challenge) : challenge(challenge) {}

Challenge* Section::getChallenge(){
    return &challenge;
}

void Section::setCompleted(bool completed){
    isSectionCompleted = completed;
}

bool Section::isComplete(){
    return isSectionCompleted;
}
