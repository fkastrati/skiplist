# Compiler and flags
CXX = clang++
CXXFLAGS = -std=c++23 -O3 -Wall -Wextra

# Target executable name
TARGET = skiplist_bench

# Source and header files
SRCS = main.cc
DEPS = skiplist.hh

# Default rule
all: $(TARGET)

$(TARGET): $(SRCS) $(DEPS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

# Clean rule to remove the executable
clean:
	rm -f $(TARGET)

# Rule to run the benchmark
run: all
	./$(TARGET)
