/**
 * Description: header file for the challenge class, which represents a coding challenge with an ability to check if a solution is valid
 * Authors: Matthew Ashton Knochel, Carter Dean, Abdulla Alnuaimi, Logan Luker, Aidan Sheehan
*/

#ifndef CHALLENGE_H
#define CHALLENGE_H

#include <string>

class Challenge
{
private:
    //string representing code to run before the user's code
    std::string beforeCode = "";
    //string representing code to run after the user's code
    std::string afterCode = "";

    /**
     * pointer to a validity function to check whether or not the code is a valid solution
     **/
    bool (*validityFunction)(std::string) = [](std::string){return true;};
public:
    /**
     * @brief Challenge default constructor that creates a challenge with default parameters
     */
    Challenge();
    /**
     * @brief Challenge constructor that allows the parameters to be set to whatever value
     * @param beforeCode code to run before user code
     * @param afterCode code to run after user code
     */
    Challenge(std::string beforeCode, std::string afterCode, bool (*validityFunction)(std::string middleCode));

    /**
     * @brief executeCode method to run the code and call the validity function, if the bool is true
     * @param middleCode user's code
     * @param checkSolutionValidity run the validityfunction if true, don't otherwise
     * @return a tuple containing the output from running the code and a bool of whether or not the solution is valid
     */
    std::tuple<std::string, bool> executeCode(std::string middleCode, bool checkSolutionValidity);
};

#endif // CHALLENGE_H
