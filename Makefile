CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude # -g for debugging, -Iinclude for headers

SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=build/%.o)
TARGET = bin/mydb
DB_FILE = simple.db

$(shell mkdir -p build)
$(shell mkdir -p bin)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build/* $(TARGET) $(DB_FILE)

run: $(TARGET)
	./$(TARGET) $(DB_FILE)

test: $(TARGET)
	bundle exec rspec -fd

bear: 
	make clean
	bear -- make

.PHONY: clean run bear test
