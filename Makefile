CC = gcc
CFLAGS = -std=c99 -Wextra -Wall -Werror -pedantic -D _POSIX_C_SOURCE=200809L
DEBUG = -g

# Test parts
TEST = test
TEST_WRITE = write_simple

# Object files
OBJ = src/my_strace.o src/hash_map.o src/parse_syscall.o src/hash.o
OBJ_TEST = tests/$(TEST_WRITE).o

# main executable
TARGET = my_strace



all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(TEST_WRITE): $(OBJ_TEST)
	$(CC) $(DEBUG) $(CFLAGS) -o $@ $^

$(TEST): $(TEST_WRITE)
	cat tests/write_simple.c
	./$(TARGET) ./write_simple

clean:
	$(RM) $(TARGET) $(TEST_WRITE) $(OBJ) $(OBJ_TEST)
