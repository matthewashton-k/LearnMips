/**
 * Description: header file for a static class that stores each of the validity functions for the challenges in each section of the Learn Mips app
 * Authors: Matthew Ashton Knochel, Carter Dean, Abdulla Alnuaimi, Logan Luker, Aidan Sheehan
*/

#ifndef VALIDITYFUNCTIONS_H
#define VALIDITYFUNCTIONS_H
#include <string>

class ValidityFunctions
{
public:
    static bool section1ValidityFunction(std::string);
    static bool section2ValidityFunction(std::string);
    static bool section3ValidityFunction(std::string);
    static bool section4ValidityFunction(std::string);
    static bool section5ValidityFunction(std::string);
    static bool section6ValidityFunction(std::string);
    static bool section7ValidityFunction(std::string);
    static bool section8ValidityFunction(std::string);
    static bool section9ValidityFunction(std::string);
    static bool section10ValidityFunction(std::string);
    static bool section11ValidityFunction(std::string);
};

#endif // VALIDITYFUNCTIONS_H
