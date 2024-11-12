//
// Created by 1234 on 9/20/24.
//

#include <DictFile.h>
#include <sstream>
#include <iostream>
#include <iomanip> 
#include <Utils.h>

DictFile::DictFile() {
    num_records = -1;
    record_size = 65;
}

DictFile::~DictFile() = default;

bool DictFile::openForWrite(const std::string& file_path) {
    this->dict_file.open(file_path, std::ios::out);
    this->num_records = Utils::getNumRecords(this->dict_file, record_size);
    return true;
}

bool DictFile::openForRead(const std::string& file_path) {
    this->dict_file.open(file_path, std::ios::in);
    this->num_records = Utils::getNumRecords(this->dict_file, record_size);
    return true;
}

void DictFile::closeAfterWrite() {
    this->num_records = -1;
    this->dict_file.close();
}

void DictFile::closeAfterRead() {
    this->num_records = -1;
    this->dict_file.close();
}

bool DictFile::writeRecord(const std::string& terms, int num_docs, int start) {
    if(!dict_file.is_open()) {
        std::cerr << "Unable to write record, map file currently closed" << std::endl;
        return false;
    }

    this->dict_file
    << std::setw(45) << std::left << terms
    << std::setw(9) << std::left << num_docs
    << std::setw(10) << std::left << start
    << std::endl;

    this->num_records += 1;
    return true;
}

bool DictFile::readRecord(int record_num, std::string& terms, int& num_docs, int& start) {
    terms = "-1";
    num_docs = -1;
    start = -1;

    if(!dict_file.is_open()) {
        std::cerr << "Unable to read record, map file currently closed" << std::endl;
        return false;
    }

    if(this->num_records <= record_num) {
        return false;
    }

    this->dict_file.seekg(record_size * record_num, std::ios::beg);
    std::string line;
    getline(this->dict_file, line);
    std::istringstream stream(line);
    stream >> terms;
    stream >> num_docs;
    stream >> start;
    return true;
}