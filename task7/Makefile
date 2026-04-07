CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
TARGET = task7
SRC = main.c copy_fork.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	$(RM) $(TARGET) $(TARGET).exe
