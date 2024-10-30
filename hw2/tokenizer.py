import os
import re

# This function converts a string to lowercase and returns it
def downcase_and_convert(s):
    result = ''
    for ch in s:
        if 'A' <= ch <= 'Z':
            result += chr(ord(ch) + 32)  # Convert uppercase to lowercase
        else:
            result += ch
    return result

# This function extracts and processes the contents of HTML attributes like 'alt' and 'contents'
def extract_html_attribute_contents(s, match, outfile):
    p1 = 0
    p2 = 0

    # Searching for the pattern (e.g., 'alt="')
    while p1 < len(s) and p2 < len(match):
        if s[p1] == match[p2]:
            p1 += 1
            p2 += 1
        else:
            p1 += 1
            p2 = 0  # Reset p2 if it's not a match

    if p2 == len(match):  # If we found the match
        curr = ""  # Use a string to store the current word

        # Process the content of the attribute
        while p1 < len(s) and s[p1] != '"':
            if s[p1] == ' ':  # When encountering a space, write the accumulated word
                if curr:
                    outfile.write(downcase_and_convert(curr) + '\n')
                curr = ""
            else:
                curr += s[p1]
            p1 += 1

        if curr:
            outfile.write(downcase_and_convert(curr) + '\n')

# Handle HTML opening tags and extract relevant attributes
def handle_html_open_tags(s, outfile):
    extract_html_attribute_contents(s, 'alt="', outfile)
    extract_html_attribute_contents(s, 'contents="', outfile)

# Function to process URLs
def match_urls(text, outfile):
    outfile.write(text + '\n')

# Tokenization patterns and processing logic
html_comment_pattern = re.compile(r'<!--(.|\n)*?-->')
html_opening_tags_pattern = re.compile(r'<[a-zA-Z][^>/]*>')
html_closing_tags_pattern = re.compile(r'</[a-zA-Z][^>/]*>')
urls_pattern = re.compile(r'https?://[a-zA-Z0-9\-\.]+\.[a-zA-Z]{2,6}(/[a-zA-Z0-9\-._~\/]*)?')
word_pattern = re.compile(r'[A-Za-z][a-z]*')

# Function to tokenize and process text based on the patterns
def tokenize_and_process(text, outfile):
    # Ignore HTML comments and closing tags
    if html_comment_pattern.match(text) or html_closing_tags_pattern.match(text):
        return
    
    # Match URLs
    if urls_pattern.match(text):
        match_urls(text, outfile)
        return
    
    # Match opening HTML tags
    if html_opening_tags_pattern.match(text):
        handle_html_open_tags(text, outfile)
        return

    # Match and process words with length > 1
    if word_pattern.match(text) and len(text) > 1:
        outfile.write(downcase_and_convert(text) + '\n')

# Function to process input files and write the output
def process_directory(input_dir, output_dir):
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    for filename in os.listdir(input_dir):
        if filename.startswith('.'):
            continue  # Skip hidden files

        in_filename = os.path.join(input_dir, filename)
        print(f"\nProcessing file {in_filename}")

        # Reading input file
        with open(in_filename, 'r') as infile:
            text = infile.read()

        out_filename = os.path.join(output_dir, filename.replace(".html", ".out"))
        with open(out_filename, 'w') as outfile:
            for line in text.splitlines():
                tokenize_and_process(line, outfile)

# Simulate the yywrap function
def yywrap(input_dir, output_dir):
    process_directory(input_dir, output_dir)
    print("Done tokenizing.")

# Simulate main function behavior
if __name__ == '__main__':
    import sys
    if len(sys.argv) != 3:
        print("Incorrect number of arguments.")
        sys.exit(1)

    in_dirname = sys.argv[1]
    out_dirname = sys.argv[2]

    if not os.path.isdir(in_dirname):
        print(f"Unable to open input directory: {in_dirname}")
    else:
        yywrap(in_dirname, out_dirname)
