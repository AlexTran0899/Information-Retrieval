//
// Created by Alex Tran on 2/17/24.
//

#include "Utils.h"
#include <algorithm>

std::string Utils::replaceSpacesWithUnderScore(const std::string& s) {
    std::string result = s;
    std::replace(result.begin(), result.end(), ' ', '_');
    return result;
}

std::string Utils::replaceUnderScoreWithSpaces(const std::string& s) {
    std::string result = s;
    std::replace(result.begin(), result.end(), '_', ' ');
    return result;
}

bool Utils::isBlankRecord(const std::string &line) {
    return stoi(line) == -1;
}