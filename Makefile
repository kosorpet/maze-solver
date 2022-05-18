#Set compiler
CXX = g++
CXXFLAGS = -Wall -pedantic -lSDL2 -std=c++14

#Set sources and object files
SOURCES = $(wildcard src/*.cpp)
HEADERS = $(wildcard src/*.h)
OBJECTS = $(SOURCES:src/%.cpp = build/%.o)


.PHONY: compile
compile: solver

solver: build/main
	cp $< $@

.PHONY: clean
clean:
	rm -rf build/ solver  2>/dev/null	

build/main: $(OBJECTS)
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@

build/%.o: src/%.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $< -c -o $@
