import sys
import os


input_file = sys.argv[1]
output_directory = sys.argv[2] if(len(sys.argv) > 2) else "."

readFile = open(input_file, mode="r")
newFileName = input_file.replace(".html", ".txt")

writeFile = open(os.path.join(output_directory, newFileName), mode="w")

for line in readFile:
    temp = line.split()
    for word in temp:
        writeFile.write(word + "\n")

readFile.close()
writeFile.close()


