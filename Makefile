CC = gcc
CFLAGS = -std=c99 -Wextra -Wall -Werror -pedantic -g

OBJ = my_strace.o
TARGET = my_strace

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) $(TARGET) $(OBJ)
