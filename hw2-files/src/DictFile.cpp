//
// Created by 1234 on 9/20/24.
//

#include <DictFile.h>
#include <sstream>
#include <iostream>
#include <iomanip> 

DictFile::DictFile() {
    num_records = -1;
    record_size = 35;
}

DictFile::~DictFile() = default;

bool DictFile::openForWrite(const std::string& filename) {
    this->num_records = 0;
    this->dict_file.open("/home/phtran/Information-Retrieval/hw2-files/Data/" + filename + ".data", std::ios::out | std::ios::app);
    return true;
}

bool DictFile::openForRead(const std::string& filename) {
    this->dict_file.open("/home/phtran/Information-Retrieval/hw2-files/Data/" + filename + ".data", std::ios::in);
    return true;
}

void DictFile::closeAfterWrite() {
    this->dict_file.close();
}

void DictFile::closeAfterRead() {
    this->dict_file.close();
}

bool DictFile::writeRecord(const std::string& terms, int num_docs, int start) {
    if(!dict_file.is_open()) {
        std::cerr << "Unable to write record, map file currently closed" << std::endl;
        return false;
    }

    this->dict_file
    << std::setw(14) << std::left << terms
    << std::setw(10) << std::left << num_docs
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