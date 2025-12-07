# -----------------------------
# Makefile for Mini Engine (main.cpp + ImGui)
# -----------------------------

# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 \ -I$(SRC_DIR) -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends

# Source directories
SRC_DIR = mini_engine/src
IMGUI_DIR = imgui

# Engine sources
ENGINE_SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:.cpp=.o)

# ImGui sources
IMGUI_SRC = $(IMGUI_DIR)/imgui.cpp \
            $(IMGUI_DIR)/imgui_draw.cpp \
            $(IMGUI_DIR)/imgui_widgets.cpp \
            $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp \
            $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp

# All sources
SRC = $(ENGINE_SRC) $(IMGUI_SRC)

# Output executable
OUT = $(SRC_DIR)/main.out
# For Windows (MinGW)
# OUT = $(SRC_DIR)/engine.exe

# Libraries (Linux / WSL)
LIBS = -lglfw -lGLEW -lGL -ldl -lpthread -lm

# For Windows (MinGW)
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
	rm -f $(OBJ)/*.o $(OUT)

# Run the program
run: $(OUT)
	./$(OUT)


# -----------------------------
# Makefile for Mini Engine (main.cpp)
# -----------------------------

# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -I$(SRC_DIR)

# Source and output
SRC_DIR = mini_engine/src
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:.cpp=.o)

# Output executable
OUT = $(SRC_DIR)/main.out
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
	rm -f $(OBJ) $(OUT)

# Run the program
run: $(OUT)
	./$(OUT)

