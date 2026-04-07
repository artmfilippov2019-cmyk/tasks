CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
TARGET = task4
SRC = main.c options_helpers.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	$(RM) $(TARGET) $(TARGET).exe
