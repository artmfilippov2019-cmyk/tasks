CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
TARGET = task2
SRC = main.c fifo_chat.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	$(RM) $(TARGET) $(TARGET).exe
