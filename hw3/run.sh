#!/bin/bash
make
echo "Running..."
# Read arguments from the file and pass them to the program
CONFIG=$(<CONFIG)

# Run the program with the arguments
./output $CONFIG