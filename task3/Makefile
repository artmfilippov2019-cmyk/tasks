CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
TARGET = task3
SRC = main.c line_sort.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	$(RM) $(TARGET) $(TARGET).exe
