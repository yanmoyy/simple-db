CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude # -g for debugging, -Iinclude for headers

SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=build/%.o)
TARGET = bin/mydb

$(shell mkdir -p build)
$(shell mkdir -p bin)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build/* $(TARGET)

run: $(TARGET)
	./$(TARGET)

bear: 
	make clean
	bear -- make

.PHONY: clean run bear
