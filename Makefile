CC = gcc
CFLAGS = -m32 -std=c99 -Wextra -Wall -Werror -pedantic -D _POSIX_C_SOURCE

OBJ = src/my_strace.o
OBJ_TEST = tests/write_simple.o
TARGET = my_strace
TEST = test
TEST_WRITE = write_simple


all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(TEST_WRITE): $(OBJ_TEST)
	$(CC) $(CFLAGS) -o $@ $^

$(TEST): $(TEST_WRITE)
	cat tests/write_simple.c
	./$(TARGET) tests/write_simple

clean:
	$(RM) $(TARGET) $(TEST_WRITE) $(OBJ) $(OBJ_TEST)
