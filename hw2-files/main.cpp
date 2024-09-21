#include <iostream>
#include "./Include/InvertedFile.h"

int main() {
    InvertedFile invFile;

    // Step 1: Open files for writing
    std::cout << "Opening files for writing..." << std::endl;
    if (!invFile.openForWrite()) {
        std::cerr << "Error: Could not open files for writing!" << std::endl;
        return 1;
    }

    // Step 2: Write records to mapFile, dictFile, and postFile
    std::cout << "Writing records..." << std::endl;
    invFile.writeMapRecord("Document1");
    invFile.writeMapRecord("Document2");

    invFile.writeDictRecord("term1", 2, 0);
    invFile.writeDictRecord("term2", 3, 5);

    invFile.writePostRecord(0, 1);
    invFile.writePostRecord(1, 2);

    // Step 3: Close files after writing
    invFile.closeAfterWrite();
    std::cout << "Records written and files closed." << std::endl;

    // Step 4: Open files for reading
    std::cout << "Opening files for reading..." << std::endl;
    if (!invFile.openForRead()) {
        std::cerr << "Error: Could not open files for reading!" << std::endl;
        return 1;
    }

    // Step 5: Read records from mapFile, dictFile, and postFile
    std::string document_name;
    std::cout << "Reading mapFile records..." << std::endl;
    invFile.readMapRecord(0, document_name);
    std::cout << "Map Record 0: " << document_name << std::endl;

    invFile.readMapRecord(1, document_name);
    std::cout << "Map Record 1: " << document_name << std::endl;

    invFile.readMapRecord(2, document_name);
    std::cout << "Map Record 2: " << document_name << std::endl;

    std::string terms;
    int num_docs, start;
    std::cout << "Reading dictFile records..." << std::endl;
    invFile.readDictRecord(0, terms, num_docs, start);
    std::cout << "Dict Record 0: term=" << terms << ", num_docs=" << num_docs << ", start=" << start << std::endl;

    invFile.readDictRecord(1, terms, num_docs, start);
    std::cout << "Dict Record 1: term=" << terms << ", num_docs=" << num_docs << ", start=" << start << std::endl;

    invFile.readDictRecord(2, terms, num_docs, start);
    std::cout << "Dict Record 2: term=" << terms << ", num_docs=" << num_docs << ", start=" << start << std::endl;

    int doc_id, freq;
    std::cout << "Reading postFile records..." << std::endl;
    invFile.readPostRecord(0, doc_id, freq);
    std::cout << "Post Record 0: doc_id=" << doc_id << ", freq=" << freq << std::endl;

    invFile.readPostRecord(1, doc_id, freq);
    std::cout << "Post Record 1: doc_id=" << doc_id << ", freq=" << freq << std::endl;

    invFile.readPostRecord(2, doc_id, freq);
    std::cout << "Post Record 2: doc_id=" << doc_id << ", freq=" << freq << std::endl;

    // Step 6: Close files after reading
    invFile.closeAfterRead();
    std::cout << "Files closed after reading." << std::endl;

    return 0;
}