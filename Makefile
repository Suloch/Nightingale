.PHONY: default all test clean

TARGET_EXEC = test
LIBS = -lfl -lm
CC = g++
CFLAGS = -Wall

BUILD_DIR = ./build

ENGINE_DIR := ./nightingale
APP_DIR := ./test_app

SRC_DIR := $(ENGINE_DIR) $(APP_DIR)

SRCS := $(shell find $(SRC_DIRS) -name '*.cpp')

OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)

INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

CPPFLAGS := $(INC_FLAGS) $(CFLAGS) $(LDFLAGS) -MMD -MP 


$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Build step for C++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	rm -r $(BUILD_DIR)


-include $(DEPS)