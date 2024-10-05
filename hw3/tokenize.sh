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

rm "$output_directory"/*.out
flex tokenizer.l
g++ lex.yy.c -o tokenizer -lfl
./tokenizer "$input_directory" "$output_directory"

# #!/bin/bash
# make
# echo "Running..."
# # Read arguments from the file and pass them to the program
# CONFIG=$(<CONFIG)

# # Run the program with the arguments
# ./output $CONFIG
# echo '----done'
