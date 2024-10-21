#include <iostream>
#include <InvertedFile.h>
#include <unordered_map>
#include <map>
#include <iostream>
#include <dirent.h>
#include <vector>
#include <algorithm>

std::unordered_map<std::string, int> global_ht_freq;
std::map<std::string, std::vector<std::pair<int,int>>> global_ht_doc_id;
std::vector<std::string> doc_name_collection;

std::pair<std::string, std::string> parse_arg(std::string arg) {
    std::pair<std::string, std::string> pair;
    int i = 0;
    for(i; i < arg.size(); i++) {
        if(arg[i] == '=') break;
        pair.first += arg[i];
    }
    i += 1;
    for(i; i < arg.size(); i++) {
        pair.second += arg[i];
    }
    return pair;
}

int largest_doc_ht_size = 0;

bool processFile(std::ifstream &file, std::string & file_name) {
    std::string line;
    std::unordered_map<std::string, int> local_ht_freq;
    int doc_id = doc_name_collection.size();
    doc_name_collection.push_back(file_name);

    while (std::getline(file, line)) {
        global_ht_freq[line] += 1;
        local_ht_freq[line] += 1;
    }

    for(const auto & pair : local_ht_freq) {
        global_ht_doc_id[pair.first].push_back({doc_id, pair.second});
    }

    largest_doc_ht_size = std::max(largest_doc_ht_size, (int)local_ht_freq.size());
    
    return true; // Return true if the file was processed successfully
}

bool processFilesInFolder(const std::string & folderPath,const InvertedFile & invFile) {
    DIR *dir;
    struct dirent *entry;

    // Open the directory
    if ((dir = opendir(folderPath.c_str())) != NULL) {
        // Loop over all the files in the directory
        while ((entry = readdir(dir)) != NULL) {
            std::string fileName = entry->d_name;

            // Skip the "." and ".." entries
            if (fileName != "." && fileName != "..") {
                std::string filePath = folderPath + fileName;

                // Try opening the file
                std::ifstream file(filePath);
                if (file.is_open()) {
                    std::cout << "Opened file: " << filePath << std::endl;
                    // Do something with the file
                    processFile(file, fileName);
                    file.close();
                } else {
                    std::cerr << "Failed to open file: " << filePath << std::endl;
                    closedir(dir);
                    return false; // Return false if any file fails to open
                }
            }
        }
        closedir(dir); // Close the directory
        return true; // Successfully processed all files
    } else {
        std::cerr << "Could not open directory: " << folderPath << std::endl;
        return false;
    }
}

int djb2_hash(const std::string &str, int array_size) {
    unsigned long hash = 5381;
    for (char c : str) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash % array_size;  // Ensure the hash is within array bounds
}

struct DictRecord {
    std::string term;
    int num_docs;
    int start;

    // Constructor with default "empty" values
    DictRecord(const std::string& t = "", int n = -1, int s = -1)
        : term(t), num_docs(n), start(s) {}

    // Method to check if this record is empty
    bool isEmpty() const {
        return term.empty() && num_docs == -1 && start == -1;
    }
};

int main(int argc, char* argv[]) {
    InvertedFile invFile;
    std::unordered_map<std::string, std::string> env;

   for (int i = 0; i < argc; ++i) {
        std::pair<std::string, std::string> pair = parse_arg(argv[i]);
        env[pair.first] = pair.second;
    }

    std::string InDirName = env["INPUT"];
    std::string OutDirName = env["DATA"];
    processFilesInFolder(InDirName, invFile);

    invFile.openForWrite(OutDirName);

    int start = 0;
    std::vector<DictRecord> dict_file_arr;

    // write to the map table
    for(int i = 0; i < doc_name_collection.size(); i++) {
        invFile.writeMapRecord(doc_name_collection[i]);
    }

    for(const auto & pair : global_ht_doc_id) {
        // prepare to write to the dict file
        dict_file_arr.emplace_back(pair.first, pair.second.size(), start);

        // write to the posting file
        for(const auto & item : pair.second) {
            start += 20;
            invFile.writePostRecord(item.first, item.second, pair.first);
        }
    }

    std::vector<DictRecord> dict_file_hash(dict_file_arr.size() * 3);

    for(const auto & item : dict_file_arr) {
        int hash_index = djb2_hash(item.term, dict_file_hash.size());

        // linear probing
        while(!dict_file_hash[hash_index].isEmpty()) {
            hash_index += 1;
            hash_index %= dict_file_hash.size();
        }
        
        dict_file_hash[hash_index] = item;
    }

    for(const auto & item : dict_file_hash) {
        if(item.isEmpty()) {
            invFile.writeDictRecord("BLANK", -1, -1);    
        } else {
            invFile.writeDictRecord(item.term, item.num_docs, item.start);
        }
    }

    std::cout << "Unique Token Count: " << global_ht_freq.size() << std::endl;
    std::cout << "largest_doc_ht size: " << largest_doc_ht_size << std::endl;

    // // Step 1: Open files for writing
    // std::cout << "Opening files for writing..." << std::endl;
    // if (!invFile.openForWrite(env["DATA"])) {
    //     std::cerr << "Error: Could not open files for writing!" << std::endl;
    //     return 1;
    // }

    // // Step 2: Write records to mapFile, dictFile, and postFile
    // std::cout << "Writing records..." << std::endl;
    // invFile.writeMapRecord("Document1");
    // invFile.writeMapRecord("Document2");

    // invFile.writeDictRecord("term1", 2, 0);
    // invFile.writeDictRecord("term2", 3, 5);

    // invFile.writePostRecord(0, 1);
    // invFile.writePostRecord(1, 2);

    // // Step 3: Close files after writing
    // invFile.closeAfterWrite();
    // std::cout << "Records written and files closed." << std::endl;

    // // Step 4: Open files for reading
    // std::cout << "Opening files for reading..." << std::endl;
    // if (!invFile.openForRead(env["DATA"])) {
    //     std::cerr << "Error: Could not open files for reading!" << std::endl;
    //     return 1;
    // }

    // // Step 5: Read records from mapFile, dictFile, and postFile
    // std::string document_name;
    // std::cout << "Reading mapFile records..." << std::endl;
    // invFile.readMapRecord(0, document_name);
    // std::cout << "Map Record 0: " << document_name << std::endl;

    // invFile.readMapRecord(1, document_name);
    // std::cout << "Map Record 1: " << document_name << std::endl;

    // invFile.readMapRecord(2, document_name);
    // std::cout << "Map Record 2: " << document_name << std::endl;

    // std::string terms;
    // int num_docs, start;
    // std::cout << "Reading dictFile records..." << std::endl;
    // invFile.readDictRecord(0, terms, num_docs, start);
    // std::cout << "Dict Record 0: term=" << terms << ", num_docs=" << num_docs << ", start=" << start << std::endl;

    // invFile.readDictRecord(1, terms, num_docs, start);
    // std::cout << "Dict Record 1: term=" << terms << ", num_docs=" << num_docs << ", start=" << start << std::endl;

    // invFile.readDictRecord(2, terms, num_docs, start);
    // std::cout << "Dict Record 2: term=" << terms << ", num_docs=" << num_docs << ", start=" << start << std::endl;

    // int doc_id, freq;
    // std::cout << "Reading postFile records..." << std::endl;
    // invFile.readPostRecord(0, doc_id, freq);
    // std::cout << "Post Record 0: doc_id=" << doc_id << ", freq=" << freq << std::endl;

    // invFile.readPostRecord(1, doc_id, freq);
    // std::cout << "Post Record 1: doc_id=" << doc_id << ", freq=" << freq << std::endl;

    // invFile.readPostRecord(2, doc_id, freq);
    // std::cout << "Post Record 2: doc_id=" << doc_id << ", freq=" << freq << std::endl;

    // // Step 6: Close files after reading
    // invFile.closeAfterRead();
    // std::cout << "Files closed after reading." << std::endl;

    return 0;
}