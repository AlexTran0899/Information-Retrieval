#!/bin/bash
echo '----running'

input_directory="$1"
output_directory="$2"

# Ensure the input directory has a trailing slash
if [[ ! "$input_directory" == */ ]]; then
    input_directory="$input_directory/"
fi

echo "Input Directory: $input_directory"
echo "Output Directory: $output_directory"

for FILE in "$input_directory"*; do 
    # Check if the file is not the script itself, is a regular file, and has a .html extension
    if [[ "$FILE" != "$0" && -f "$FILE" && "${FILE##*.}" == "html" ]]; then
        python tokenizer.py "$FILE" "$output_directory" 
    fi
done

echo '----done'
