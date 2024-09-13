#!/bin/bash
echo '----running'

input_directory="$1"
output_directory="$2"

# Ensure the input directory has a trailing slash
if [[ ! "$input_directory" == */ ]]; then
    input_directory="$input_directory/"
fi

# Ensure the output directory exists
mkdir -p "$output_directory"

echo "Input Directory: $input_directory"
echo "Output Directory: $output_directory"

# Check if the input directory is empty
if [ -z "$(ls -A "$input_directory")" ]; then
    echo "Input directory is empty!"
    exit 1
fi

python3 tokenizer.py "$input_directory" "$output_directory" 2>>error_log.txt

# Concatenate all the output .txt files into one file
cat "$output_directory"/*.out > "$output_directory/all_tokens.txt"

# Sort the tokens, count unique occurrences, and store in a new file
sort "$output_directory/all_tokens.txt" | uniq -c > "$output_directory/unique_terms.txt"

# Create alpha.txt by sorting terms alphabetically
sort -k2 "$output_directory/unique_terms.txt" > "$output_directory/alpha.txt"

# Create freqs.txt by sorting terms by frequency (numerically, decreasing order)
sort -nr "$output_directory/unique_terms.txt" > "$output_directory/freq.txt"

echo '----done'
