.PHONY: default all test clean

TARGET_EXEC = test
LIBS = -lfl -lm
CFLAGS = -Wall

BUILD_DIR = ./build

ENGINE_DIR := ./nightingale
APP_DIR := ./test_app

SRC_DIR := $(ENGINE_DIR) $(APP_DIR)


SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' )


OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)

INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

CPPFLAGS := $(INC_FLAGS) $(CFLAGS) $(LDFLAGS) -MMD -MP -g


$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)
	mkdir -p $(BUILD_DIR)/$(ENGINE_DIR)/render/shader/default/


# Build step for C++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


clean:
	rm -r $(BUILD_DIR)

shader:
	glslc $(ENGINE_DIR)/render/shader/default/shader.vert -o $(BUILD_DIR)/$(ENGINE_DIR)/render/shader/default/vert.spv
	glslc $(ENGINE_DIR)/render/shader/default/shader.frag -o $(BUILD_DIR)/$(ENGINE_DIR)/render/shader/default/frag.spv


-include $(DEPS)