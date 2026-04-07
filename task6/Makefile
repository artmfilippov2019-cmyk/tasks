CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2 -D_GNU_SOURCE
TARGET = task6
SRC = main.c file_type.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	$(RM) $(TARGET) $(TARGET).exe
