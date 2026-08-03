CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -IInc
TARGET = Build/DigitalClock
SOURCES = $(wildcard Src/*.cpp)
OBJECTS = $(SOURCES:Src/%.cpp=Build/%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	mkdir -p Build
	$(CXX) $(OBJECTS) -o $@

Build/%.o: Src/%.cpp
	mkdir -p Build
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf Build/*

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run