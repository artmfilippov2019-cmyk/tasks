CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
LDFLAGS = -pthread
TARGET = task1
SRC = main.c parallel_sort.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	$(RM) $(TARGET) $(TARGET).exe
