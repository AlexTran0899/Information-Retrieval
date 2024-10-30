//
// Created by 1234 on 9/20/24.
//
#include <InvertedFile.h>
InvertedFile::InvertedFile() {

}

InvertedFile::~InvertedFile() {

}

bool InvertedFile::openForRead(const std::string & path) {
    const bool dict_open = dictFile.openForRead(path + "dict.data");
    const bool map_open = mapFile.openForRead(path + "map.data");
    const bool post_open = postFile.openForRead(path + "post.data");
    return dict_open && map_open && post_open;
}

bool InvertedFile::openForWrite(const std::string & path) {
    const bool dict_open = dictFile.openForWrite(path + "dict.data");
    const bool map_open = mapFile.openForWrite(path + "map.data");
    const bool post_open = postFile.openForWrite(path + "post.data");
    return dict_open && map_open && post_open;
}
void InvertedFile::closeAfterRead() {
    dictFile.closeAfterRead();
    mapFile.closeAfterRead();
    postFile.closeAfterRead();
}
void InvertedFile::closeAfterWrite() {
    dictFile.closeAfterWrite();
    mapFile.closeAfterWrite();
    postFile.closeAfterWrite();
}

bool InvertedFile::writeMapRecord(const std::string& document_name) {
    mapFile.writeRecord(document_name);
    return true;
}
bool InvertedFile::writeDictRecord(const std::string& terms, int num_docs, int start) {
    dictFile.writeRecord(terms, num_docs, start);
    return true;
}
bool InvertedFile::writePostRecord(const int& doc_id, const int& freq) {
    postFile.writeRecord(doc_id, freq);
    return true;
}

bool InvertedFile::readMapRecord(int record_num, std::string& document_name) {
    std::string doc_name;
    mapFile.readRecord(record_num, doc_name);
    std::cout << "doc_name is: "<< doc_name << std::endl;
    document_name = doc_name;
    return true;
}
bool InvertedFile::readDictRecord(int record_num, std::string& terms, int& num_docs, int& start) {
    dictFile.readRecord(record_num, terms, num_docs, start);
    return true;
}
bool InvertedFile::readPostRecord(int record_num, int& doc_id, int& freq) {
    postFile.readRecord(record_num, doc_id, freq);
    return true;
}