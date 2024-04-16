#ifndef SECTION_H
#define SECTION_H

#include "challenge.h"

class Section
{
private:
    Challenge challenge;
    bool isSectionCompleted = false;
public:
    Section();
    Section(Challenge challenge);

    Challenge* getChallenge();
    void setCompleted(bool completed);
};

#endif // SECTION_H
