//
// Created by 1234 on 9/20/24.
//

#ifndef MAPFILE_H
#define MAPFILE_H
#include <fstream>
#include <iostream>
#include <Utils.h>

#endif //MAPFILE_H
class MapFile {
private:
    std::fstream map_file;
    int record_size;
    int num_records;

public:
    MapFile();
    ~MapFile();
    bool openForWrite(const std::string& filename);
    bool openForRead(const std::string& filename);
    void closeAfterWrite();
    void closeAfterRead();
    bool writeRecord(const std::string& document_name);
    bool readRecord(int record_num, std::string& document_name);

};
