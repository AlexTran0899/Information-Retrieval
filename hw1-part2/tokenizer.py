import spacy
import sys
import glob
import os.path
from spacy.matcher import Matcher
import time

def match_uppercase(matcher, doc, i, matches):
    match_id, start, end = matches[i]
    token = doc[start]
    if not token._.matched:
        token._.modified_text = token.text.lower()
        token._.matched = True

def match_digit(matcher, doc, i, matches):
    match_id, start, end = matches[i]
    token = doc[start]
    if not token._.matched:
        token._.matched = True

def match_default(matcher, doc, i, matches):
    match_id, start, end = matches[i]
    token = doc[start]
    if not token._.matched:
        token._.matched = True


# Add a custom extension to tokens to track if they have been matched
spacy.tokens.Token.set_extension("matched", default=False)
spacy.tokens.Token.set_extension('modified_text', default=None, force=True)

# Define matching patterns
token_patterns = [
    [{"TEXT": {"REGEX": r"[0-9]+"}}],    # Number match
    [{"TEXT": {"REGEX": r"[A-Z]"}}],     # Uppercase match
    [{"TEXT": {"REGEX": "\S+"}}],        # Any other NON-WHITESPACE token
]

# Load spaCy model with unnecessary components disabled
nlp = spacy.load("en_core_web_sm")

# Increase the max_length limit to handle larger texts
nlp.max_length = 5000000  # Adjust this value as needed
matcher = Matcher(nlp.vocab)

# Add match patterns to matcher
matcher.add("digit", [token_patterns[0]], on_match=match_digit)
matcher.add("uppercase", [token_patterns[1]], on_match=match_uppercase)
matcher.add("default", [token_patterns[2]], on_match=match_default)

# Open and read the file
def read_file(file_name):
    with open(file_name, 'r') as file:
        text = file.read()
        print("Processing file:", file_name)
        return nlp(text)

# Write tokens to a new output file with a '.out' extension
def save_tokens_to_file(file_name, token_list):
    output_file_name = os.path.basename(file_name).replace('.html', '.out')
    with open(os.path.join(outputDir, output_file_name), 'w') as output_file:
        # Process the matches
        for token in token_list:
            if len(token) > 1:
                output_file.write(f"{token}\n")

def get_html_attribute_value(text, tag):
    p1 = 0
    p2 = 0
    while(p1 < len(text)):
        if p2 == len(tag):
            break
        elif(text[p1] != tag[p2]):
            p1 += 1
            p2 = 0
            continue
        else:
            p1 += 1
            p2 += 1

    data = ''
    while(p1 < len(text)):
        if(text[p1] == '"'):
            break
        data += text[p1]
        p1 += 1

    return data

def is_float(string):
    try:
        float(string)
        return True  # Conversion successful
    except ValueError:
        return False  # Conversion failed
    
def tokenize_and_save(file_name):
    global total_token_count
    try:
        doc = read_file(file_name)
        matcher(doc)
    
        # Build the token list using the modified text
        token_list = []
        inHtml = False
        temp = ''
        
        for token in doc:
            text = token._.modified_text if token._.modified_text else token.text
            if token.text == '<':
                inHtml = True
            elif token.text == '>':
                alt = get_html_attribute_value(temp, 'alt="')
                contents = get_html_attribute_value(temp, 'contents="')
                temp = ''
                if alt:
                    token_list += alt.split(' ')
                if contents:
                    token_list += contents.split(' ')
                inHtml = False
            elif inHtml:
                temp += text + ' '
            else:
                if is_float(text):
                    continue
                elif token.is_punct:
                    continue
                elif token.is_space:
                    continue
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

file_list = glob.glob(inputDir + '/*.html')
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