/**
 * Description: header file for a class representing a section in the Learn Mips app
 * Authors: Matthew Ashton Knochel, Carter Dean, Abdulla Alnuaimi, Logan Luker, Aidan Sheehan
*/

#ifndef SECTION_H
#define SECTION_H

#include "challenge.h"

class Section
{
private:
    //a challenge associated with the section
    Challenge challenge;
    //a bool to show completion state of the section
    bool isSectionCompleted = false;
public:
    /**
     * @brief Section default constructor that creates a default challenge
     */
    Section();

    /**
     * @brief Section consturctor that allows the addition of a custom challenge
     * @param challenge the challenge to associate with the section
     */
    Section(Challenge challenge);

    /**
     * @brief getChallenge getter for challenge member
     * @return the challenge associated with this section
     */
    Challenge* getChallenge();

    /**
     * @brief setCompleted setter for completion state of the section
     * @param completed new state
     */
    void setCompleted(bool completed);

    /**
     * @brief isComplete getter for completion state
     * @return the completion state of the section
     */
    bool isComplete();
};

#endif // SECTION_H
