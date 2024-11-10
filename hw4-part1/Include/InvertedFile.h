//
// Created by 1234 on 9/20/24.
//

#ifndef INVERTEDFILE_H
#define INVERTEDFILE_H
#include "DictFile.h"
#include "MapFile.h"
#include "PostFile.h"

#endif //INVERTEDFILE_H
class InvertedFile {
private:
    MapFile mapFile;
    PostFile postFile;
    DictFile dictFile;
public:
    InvertedFile();
    ~InvertedFile();
    bool openForRead(const std::string & path);
    bool openForWrite(const std::string & path);
    void closeAfterRead();
    void closeAfterWrite();

    bool writeMapRecord(const std::string& document_name);
    bool writeDictRecord(const std::string& terms, int num_docs, int start);
    bool writePostRecord(const int& doc_id, const double& tf_idf);

    bool readMapRecord(int record_num, std::string& document_name);
    bool readDictRecord(int record_num, std::string& terms, int& num_docs, int& start);
    bool readPostRecord(int record_num, int& doc_id, int& freq);
};

