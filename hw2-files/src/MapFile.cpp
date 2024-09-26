//
// Created by 1234 on 9/20/24.
//

#include <MapFile.h>
#include <iomanip>

MapFile::MapFile() {
    num_records = -1;
    record_size = 50;
}

MapFile::~MapFile() = default;

bool MapFile::openForWrite(const std::string& filename) {
    this->num_records = 0;
    this->map_file.open("/home/phtran/Information-Retrieval/hw2-files/Data/" + filename + ".data", std::ios::out);
    return true;
}

bool MapFile::openForRead(const std::string& filename) {
    this->map_file.open("/home/phtran/Information-Retrieval/hw2-files/Data/" + filename + ".data", std::ios::in);
    return true;
}

void MapFile::closeAfterWrite() {
    this->map_file.close();
}

void MapFile::closeAfterRead() {
    this->map_file.close();
}

bool MapFile::writeRecord(const std::string& document_name) {
    if(!map_file.is_open()) {
        std::cerr << "Unable to write record, map file currently closed" << std::endl;
        return false;
    }

    this->map_file
    << std::setw(49) << std::left << Utils::replaceSpacesWithUnderScore(document_name)
    << std::endl;

    this->num_records += 1;
    return true;
}

bool MapFile::readRecord(const int record_num, std::string& document_name) {
    document_name = "-1";
    if(!map_file.is_open()) {
        std::cerr << "Unable to read record, map file currently closed" << std::endl;
        return false;
    }

    if(this->num_records <= record_num) {
        return false;
    }

    this->map_file.seekg(record_size * record_num, std::ios::beg);
    std::string line;
    getline(this->map_file, document_name);
    return true;
}