//
// Created by 1234 on 9/20/24.
//

#ifndef POSTFILE_H
#define POSTFILE_H
#include <fstream>
#include <iostream>

#endif //POSTFILE_H
class PostFile {
private:
    std::fstream post_file;
    int record_size;
    int num_records;

public:
    PostFile();
    ~PostFile();
    bool openForWrite(const std::string& file_path);
    bool openForRead(const std::string& file_path);
    void closeAfterWrite();
    void closeAfterRead();
    bool writeRecord(const int& doc_id, const int& freq);
    bool readRecord(int record_num, int& doc_id, int& freq);

};
