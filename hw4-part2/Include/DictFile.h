//
// Created by 1234 on 9/20/24.
//

#ifndef DICTFILE_H
#define DICTFILE_H
#include <fstream>
#include <iostream>

#endif //DICTFILE_H
class DictFile {
private:
    std::fstream dict_file;
    int record_size;
    int num_records;

public:
    DictFile();
    ~DictFile();
    bool openForWrite(const std::string& filename);
    bool openForRead(const std::string& filename);
    void closeAfterWrite();
    void closeAfterRead();
    bool writeRecord(const std::string& terms, int num_docs, int start);
    bool readRecord(int record_num, std::string& terms, int& num_docs, int& start);
};
