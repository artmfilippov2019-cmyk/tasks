CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20 -O2
TARGET = task10
SRCS = main.cpp Parser.cpp tokenizer.cpp

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	$(RM) $(TARGET) $(TARGET).exe *.o
