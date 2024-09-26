PROJECT_DIR=.

CFLAGS=-I${PROJECT_DIR}/include/
CC=gcc
CXX=g++ -std=c++17
LDFLAGS=

SRC_DIR=${PROJECT_DIR}/src
OBJ_DIR=${PROJECT_DIR}/objs


all: run

run: compile

compile: main


main: ${OBJ_DIR}/DictFile.o ${OBJ_DIR}/InvertedFile.o ${OBJ_DIR}/MapFile.o ${OBJ_DIR}/PostFile.o  ${OBJ_DIR}/Utils.o
	${CXX} ${CFLAGS} $^ ${LDFLAGS} -o bin/$@ 


${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	${CXX} $(CFLAGS) -c $< -o $@