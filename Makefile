.PHONY: default all test clean

TARGET_EXEC = test
LIBS = -lfl -lm
CFLAGS = -Wall

BUILD_DIR = ./build

ENGINE_DIR := ./nightingale
APP_DIR := ./levelbuilder

SRC_DIR := $(ENGINE_DIR) $(APP_DIR)


SRCS := $(shell find $(SRC_DIR) -name '*.cpp' )


OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

INC_DIRS := $(shell find $(SRC_DIR) -type d)

INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CFLAGS = -std=c++2a -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi -lsqlite3

CPPFLAGS := $(INC_FLAGS) $(CFLAGS) $(LDFLAGS) -MMD -MP -g


$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

test:
	echo $(SRCS)

# Build step for C++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


clean:
	rm -r $(BUILD_DIR)

shader:
	mkdir -p $(BUILD_DIR)/$(ENGINE_DIR)/shader/
	glslc $(ENGINE_DIR)/shader/shader.vert -o $(BUILD_DIR)/$(ENGINE_DIR)/shader/vert.spv
	glslc $(ENGINE_DIR)/shader/shader.frag -o $(BUILD_DIR)/$(ENGINE_DIR)/shader/frag.spv

-include $(DEPS)




