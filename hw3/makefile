# Compiler settings
CXX=g++
CXXFLAGS=-std=c++11 -IInclude

# Output executable
output: main.o DictFile.o InvertedFile.o MapFile.o PostFile.o Utils.o
	$(CXX) main.o DictFile.o InvertedFile.o MapFile.o PostFile.o Utils.o -o output
	@rm -f *.o

# Individual object files and their dependencies
main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) -c src/main.cpp

DictFile.o: src/DictFile.cpp Include/DictFile.h
	$(CXX) $(CXXFLAGS) -c src/DictFile.cpp

InvertedFile.o: src/InvertedFile.cpp Include/InvertedFile.h
	$(CXX) $(CXXFLAGS) -c src/InvertedFile.cpp

MapFile.o: src/MapFile.cpp Include/MapFile.h
	$(CXX) $(CXXFLAGS) -c src/MapFile.cpp

PostFile.o: src/PostFile.cpp Include/PostFile.h
	$(CXX) $(CXXFLAGS) -c src/PostFile.cpp

Utils.o: src/Utils.cpp Include/Utils.h
	$(CXX) $(CXXFLAGS) -c src/Utils.cpp

# Clean command to remove object files and output executable
clean:
	rm -f *.o output
