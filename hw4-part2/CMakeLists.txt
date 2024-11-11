cmake_minimum_required(VERSION 3.29)
project(hw2_files)

set(CMAKE_CXX_STANDARD 17)

add_executable(hw2_files main.cpp
        Utils/Utils.h
        include/MapFile.h
        include/DictFile.h
        include/PostFile.h
        include/InvertedFile.h
        Utils/Utils.cpp
        source/MapFile.cpp
        source/DictFile.cpp
        source/PostFile.cpp
        Source/InvertedFile.cpp
)
