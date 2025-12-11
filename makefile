# Use g++ for C++ sources
CXX = g++
CXXFLAGS = -Wall -g -std=c++17

DBG = debugger
TARGET = target

DBG_SRC = debugger.cpp
TARGET_SRC = target.cpp

.PHONY: all run clean

all: $(DBG) $(TARGET)

# Build debugger (C++)
$(DBG): $(DBG_SRC)
	$(CXX) $(CXXFLAGS) -o $(DBG) $(DBG_SRC)

# Build target (C++)
$(TARGET): $(TARGET_SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(TARGET_SRC)

run: all
	./$(DBG) ./$(TARGET)

clean:
	rm -f $(DBG) $(TARGET) *.o
