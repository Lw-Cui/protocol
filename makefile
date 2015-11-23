SRC := client.cpp server.cpp aux.cpp
HEAD := aux.h
OBJ := ${patsubst %.cpp, %.o, $(SRC)}
TARGET := client.out server.out

FLAG := -ggdb -c -Wall -O3
CC := g++

all: $(TARGET)

$(TARGET): %.out: %.o aux.o
	$(CC) $^ -o $@

$(OBJ): %.o: %.cpp $(HEAD)
	$(CC) $< $(FLAG) -o $@

clean:
	rm $(OBJ) $(TARGET)

rebuild: clean all

.PHONY: rebuild clean all
