CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -IInc
TARGET = DigitalClock
OBJDIR = Obj
SOURCES = $(wildcard Src/*.cpp)
OBJECTS = $(SOURCES:Src/%.cpp=$(OBJDIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

$(OBJDIR)/%.o: Src/%.cpp
	mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run