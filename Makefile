# -----------------------------
# Makefile for Mini Engine
# -----------------------------

SRC_DIR = mini_engine/src
IMGUI_DIR = mini_engine/imgui

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 \
           -I$(SRC_DIR) \
           -I$(IMGUI_DIR) \
           -I$(IMGUI_DIR)/backends

LIBS = -lglfw -lGLEW -lGL -ldl -lpthread -lm

# ImGui source files
IMGUI_SRC = \
    $(IMGUI_DIR)/imgui.cpp \
    $(IMGUI_DIR)/imgui_draw.cpp \
    $(IMGUI_DIR)/imgui_widgets.cpp \
    $(IMGUI_DIR)/imgui_tables.cpp \
    $(IMGUI_DIR)/imgui_demo.cpp \
    $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp \
    $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp

# Your engine sources
ENGINE_SRC = $(wildcard $(SRC_DIR)/*.cpp)

OUT = mini_engine/build/main.out

all: $(OUT)

$(OUT): $(ENGINE_SRC) $(IMGUI_SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

run:
	./$(OUT)

clean:
	rm -f $(OUT)
