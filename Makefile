CXX = g++
CXXFLAGS = -std=c++17 -Wall

SRC = $(wildcard src/*.cpp)
TARGET = mips-sim

all:
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
