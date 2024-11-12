//
// Created by 1234 on 9/20/24.
//

#include <PostFile.h>
#include <sstream>
#include <iomanip>
#include <Utils.h>

PostFile::PostFile() {
    num_records = -1;
    record_size = 20;
}

PostFile::~PostFile() = default;



bool PostFile::openForWrite(const std::string& file_path) {
    this->post_file.open(file_path, std::ios::out);
    this->num_records = Utils::getNumRecords(this->post_file, record_size);
    return true;
}

bool PostFile::openForRead(const std::string& file_path) {
    this->post_file.open(file_path, std::ios::in);
    this->num_records = Utils::getNumRecords(this->post_file, record_size);
    return true;
}

void PostFile::closeAfterWrite() {
    this->num_records = -1;
    this->post_file.close();
}

void PostFile::closeAfterRead() {
    this->num_records = -1;
    this->post_file.close();
}

bool PostFile::writeRecord(const int& doc_id, const double& tf_idf) {
    if(!post_file.is_open()) {
        std::cerr << "Unable to write record, map file currently closed" << std::endl;
        return false;
    }

    this->post_file
    << std::setw(10) << std::left << doc_id
    << std::setw(9) << std::left << std::fixed << std::setprecision(6) << tf_idf
    << std::endl;

    this->num_records += 1;
    return true;
}

bool PostFile::readRecord(int record_num, int& doc_id, int& freq) {
    doc_id = -1;
    freq = -1;
    if(!post_file.is_open()) {
        std::cerr << "Unable to read record, map file currently closed" << std::endl;
        return false;
    }

    if(this->num_records <= record_num) {
        return false;
    }

    this->post_file.seekg(record_size * record_num, std::ios::beg);
    std::string line;
    getline(this->post_file, line);
    std::istringstream stream(line);
    stream >> doc_id;
    stream >> freq;
    return true;
}