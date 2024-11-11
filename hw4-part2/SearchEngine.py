import sys
import os
from typing import List, Dict, Optional, Union

def estimate_line_count(filename: str, record_size: Union[int, str]) -> int:
    file_size = os.path.getsize(filename)
    return file_size // int(record_size)

def parse_post_record(record: str) -> Dict[str, Union[int, float]]:
    doc_id = record[:10].strip()
    weight = record[10:19].strip()
    return {'doc_id': int(doc_id), 'weight': float(weight)}

def retrieve_post_record(configs: Dict[str, str], start: int, num_docs: int) -> List[Dict[str, Union[int, float]]]:
    post_file_path = configs['POST']
    post_records = []
    with open(post_file_path, "r") as file:
        file.seek(start)
        for _ in range(num_docs):
            record = file.readline()
            post_records.append(parse_post_record(record))
    return post_records

def process_term(terms: List[str]) -> List[str]:
    return [term.lower() for term in terms]  # Convert terms to lowercase

def load_config() -> Dict[str, str]:
    configs = {}
    with open("CONFIG", "r") as file:
        for line in file:
            key, value = line.split('=')
            configs[key.strip()] = value.strip()
    return configs

def djb2_hash(s: str, array_size: int) -> int:
    hash_value = 5381
    for char in s:
        hash_value = ((hash_value << 5) + hash_value) + ord(char)  # hash * 33 + ASCII value of char
    return hash_value % array_size  # Ensure the hash is within array bounds

def parse_dict_record(record: str) -> Dict[str, Union[str, int]]:
    term = record[:45].strip()
    num_docs = record[45:54].strip()
    start = record[54:].strip()
    return {'term': term, 'num_docs': int(num_docs), 'start': int(start)}

def read_dict_record(dict_file_path: str, record_num: int, record_size: int) -> Dict[str, Union[str, int]]:
    with open(dict_file_path, "r") as file:
        file.seek(record_num * record_size)
        record = file.readline()
    return parse_dict_record(record)

def find_dict_record(configs: Dict[str, str], term: str) -> Optional[Dict[str, Union[str, int]]]:
    num_dict_record = estimate_line_count(configs['DICT'], configs['DICT_RECORD_SIZE'])
    doc_id = djb2_hash(term, num_dict_record)
    dict_file_path = configs['DICT']
    dict_record_size = configs['DICT_RECORD_SIZE']


    with open(dict_file_path, "r") as file:
        file.seek(doc_id * dict_record_size)
        record = parse_dict_record(file.readline())
        record_search = 1
        
        while(record['term'] != term and record['term'] != '-1' and record_search < num_dict_record):
            doc_id += 1
            record_search += 1
            if(doc_id > num_dict_record):
                file.seek(0)
                doc_id = 0
            record = parse_dict_record(file.readline())
            
    return record
        




def retrieve_map_record(configs: Dict[str, str], doc_id:int) -> str:
    map_file_path = configs['MAP']
    map_record_size = configs['MAP_RECORD_SIZE']
    with open(map_file_path, "r") as file:
        file.seek(int(doc_id) * int(map_record_size))
        return file.readline().strip()


class Node:
    def __init__(self, doc_id: int, value: float):
        self.doc_id: int = doc_id
        self.value: float = value
        self.next: Optional['Node'] = None

    def print(self):
        print("doc_id: ", self.doc_id, "weight: ", self.value)

class Accumulator:
    def __init__(self):
        self.head: Optional[Node] = None
        self.size: int = 0

    def insert(self, doc_id: int, value: float) -> None:
        if self.head == None:
            self.head = Node(doc_id, value)
            self.size += 1
            return
        
        curr = self.head
        prev = None
        while curr is not None:
            if(curr.doc_id == doc_id):
                curr.value += value
                return
            prev = curr
            curr = curr.next
        new_node = Node(doc_id, value)

        prev.next = new_node

        self.size += 1


    def print_accumulator(self):
        curr = self.head
        while curr is not None:
            curr.print()
            curr = curr.next

    def get_top_k(self, k: int):
        acc = []
        curr = self.head
        
        while curr is not None:
            acc.append([curr.doc_id, curr.value])
            curr = curr.next
        
        acc.sort(key=lambda x: x[1], reverse=True)
        
        return acc[:k]

def main() -> None:
    if len(sys.argv) < 3:
        print("Usage: python3 SearchEngine.py <file_path> <query1> <query2> ...")
        sys.exit(1)

    configs = load_config()
    query = sys.argv[1]
    terms = process_term(sys.argv[2:])

    accumulator = Accumulator()

    if query == 'query':
        for term in terms:
            dict_record = find_dict_record(configs, term)
            if not dict_record: continue
            post_records = retrieve_post_record(configs, dict_record['start'], dict_record['num_docs'])
            
            for post_record in post_records:
                accumulator.insert(post_record['doc_id'], post_record['weight'])

    top_k_element = accumulator.get_top_k(100)
    for element in top_k_element:
        file_name = retrieve_map_record(configs, element[0])
        print("filename: ",file_name, "- weight: ", element[1])

if __name__ == "__main__":
    main()