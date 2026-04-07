CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
TARGET = task8
SRC = main.c sig_matmul.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	$(RM) $(TARGET) $(TARGET).exe
