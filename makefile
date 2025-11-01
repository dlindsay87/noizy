# Directories
SRC_DIR := src
INC_DIR := include
BUILD_DIR := build

# Source files and output
SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))
DEPS := $(OBJS:.o=.d)

# Include flags for each subdir in include/
INC_PATHS := $(shell find $(INC_DIR) -type d)
INC_FLAGS := $(addprefix -I, $(INC_PATHS))
# $(shell sdl2-config --cflags)

# Compiler and flags
CXX := g++
CXX_FLAGS := -std=c++17 -Wall

COMP_FLAGS := -ggdb3 -c
DEP_FLAGS = -MMD -MP -MF $(@:.o=.d)
LIB_FLAGS := -lSDL2 -lSDL2_ttf -lGLEW -lGL

TARGET := noizy

.PHONY: all clean

# Default rule
all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "Linking $@..."
	@$(CXX) $^ $(LIB_FLAGS) -o $@
	
# Compile source files to objects
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	@echo "Compiling $< -> $@..."
	@$(CXX) $(CXX_FLAGS) $(INC_FLAGS) $(COMP_FLAGS) $(DEP_FLAGS) $< -o $@

-include $(DEPS)

# Clean up build files
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

val:
	valgrind --log-file="valgrind_log.txt" ./$(TARGET)
