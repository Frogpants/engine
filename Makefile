# -----------------------------
# Makefile for Mini Engine
# -----------------------------

# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Source and output
SRC_DIR = mini_engine/src
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:.cpp=.o)

# Output executable
OUT = $(SRC_DIR)/essentials.out
# Use engine.exe if building for Windows with MinGW
# OUT = $(SRC_DIR)/engine.exe

# Libraries (Linux / WSL)
LIBS = -lglfw -lGLEW -lGL -ldl -lpthread -lm

# For MinGW (Windows)
# LIBS = -lglfw3 -lopengl32 -lgdi32 -luser32 -lkernel32

# -----------------------------
# Default target
# -----------------------------
all: $(OUT)

# Compile all sources into executable
$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

# Clean object files and executable
clean:
	rm -f $(SRC_DIR)/*.o $(OUT)

# Run the program
run: all
	./$(OUT)
