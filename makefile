SRC := client.cpp server.cpp aux.cpp message.cpp
HEAD := aux.h message.h
OBJ := ${patsubst %.cpp, %.o, $(SRC)}
AUX := aux.o message.o
TARGET := client.out server.out

FLAG := -ggdb -c -Wall -O3
CC := g++

all: $(TARGET)

$(TARGET): %.out: %.o $(AUX)
	$(CC) $^ -o $@

$(OBJ): %.o: %.cpp $(HEAD)
	$(CC) $< $(FLAG) -o $@

clean:
	rm $(OBJ) $(TARGET)

rebuild: clean all

.PHONY: rebuild clean all
