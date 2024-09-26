//
// Created by Alex Tran on 2/17/24.
//

#ifndef SELF_UTILS_H
#define SELF_UTILS_H
#include <iostream>

class Utils {
public:
    static std::string replaceUnderScoreWithSpaces(const std::string& s);
    static std::string replaceSpacesWithUnderScore(const std::string& s);
    static bool isBlankRecord(const std::string &line);
};


#endif //SELF_UTILS_H
