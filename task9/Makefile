CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
TARGET = task9
SRC = main.c longest_run.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	$(RM) $(TARGET) $(TARGET).exe
