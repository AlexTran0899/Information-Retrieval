import spacy
import sys
import glob
import os.path
import time
import re

# Load spaCy's blank English model (only tokenizer)
nlp = spacy.load("en_core_web_sm")
nlp.max_length = 5000000  # Adjust this value as needed

# Open and read the file
def read_file(file_name):
    with open(file_name, 'r', encoding='utf-8') as file:
        text = file.read()
        print("Processing file:", file_name)
        return nlp(text)

# Write tokens to a new output file with a '.out' extension
def save_tokens_to_file(file_name, token_list):
    output_file_name = os.path.basename(file_name).replace('.html', '.out')
    with open(os.path.join(outputDir, output_file_name), 'w', encoding='utf-8') as output_file:
        # Process the matches
        for token in token_list:
            if len(token) > 1:
                output_file.write(f"{token}\n")

def get_html_attribute_value(text, attribute_name):
    pattern = attribute_name + r'="([^"]*)"'
    match = re.search(pattern, text)
    if match:
        return match.group(1)
    else:
        return ''

def tokenize_and_save(file_name):
    global total_token_count
    try:
        doc = read_file(file_name)
        
        # Build the token list using the modified text
        token_list = []
        inHtml = False
        temp = ''
        
        for token in doc:
            text = token.text
            if text == '<':
                inHtml = True
                temp = ''  # Reset temp when starting a new tag
            elif text == '>':
                alt = get_html_attribute_value(temp, 'alt')
                contents = get_html_attribute_value(temp, 'contents')
                temp = ''
                if alt:
                    token_list += alt.split(' ')
                if contents:
                    token_list += contents.split(' ')
                inHtml = False
            elif inHtml:
                temp += text
            else:
                if token.like_num:
                    continue
                elif token.is_punct:
                    continue
                elif token.is_space:
                    continue
                else:
                    # Lowercase if the token is uppercase
                    if text.isupper():
                        text = text.lower()
                    token_list.append(text)
        
        save_tokens_to_file(file_name, token_list)
        total_token_count += len(token_list)
    except Exception as e:
        print(f"Error processing file {file_name}: {e}")

# Directory and file setup
inputDir = sys.argv[1]
outputDir = sys.argv[2]

if not os.path.exists(inputDir) or not os.path.exists(outputDir):
    print("Invalid path. Please enter a valid input and output directory.")
    quit()

file_list = glob.glob(os.path.join(inputDir, '*.html'))
total_token_count = 0

# Record the start time
start_time = time.time()

# Process each file in the input directory
for file_name in file_list:
    tokenize_and_save(file_name)

# Record the end time
end_time = time.time()

# Calculate and print the duration
print(f"Total tokens: {total_token_count}")
print("Tokenization complete. Files saved with '.out' extension.")
print(f"Processing time: {end_time - start_time:.2f} seconds")