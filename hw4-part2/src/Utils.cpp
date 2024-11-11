//
// Created by Alex Tran on 2/17/24.
//

#include <Utils.h>
#include <algorithm>
#include <fstream>

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

int Utils::getNumRecords(std::fstream & file, int record_size) {
    if (file.is_open()) {
            // Move to the end of the file to get the size
            file.seekg(0, std::ios::end); 
            std::streampos file_size = file.tellg(); // Get the file size in bytes
            
            // Return the number of records
            return static_cast<int>(file_size / record_size);
        }
        // Return -1 if the file is not open
        return -1;
}