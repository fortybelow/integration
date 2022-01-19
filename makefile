TARGET_EXEC ?= a.out

VERBOSE ?= true

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src
TEST_DIR ?= ./test

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

#INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FILES := $(shell find ./ -type f -regextype posix-egrep -regex ".*\.hpp|.*\.h")
INC_DIRS := $(shell find ./ -type f -regextype posix-egrep -regex ".*hpp|.*h" -exec dirname {} \; | uniq)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

TESTS := $(shell find $(TEST_DIR) -name *.cpp -or -name *.c -or -name *.s)
TESTS_OBJ := $(TESTS:%=$(BUILD_DIR)/%.o)

LIBS := crypto ssl pthread
LDFLAGS := $(addprefix -l,$(LIBS))

SRC_FILES_SHA = "$(shell find ./src -type f | sort | xargs shasum)"

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP
CC := g++

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS) 

# assembly
$(BUILD_DIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: SHELL:=/bin/bash   # HERE: this is setting the shell for b only
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# c++ test
$(BUILD_DIR)/%.test.cpp.o: SHELL:=/bin/bash   # HERE: this is setting the shell for b only
$(BUILD_DIR)/%.test.cpp.o: %.cpp
	echo "C++ test: '$<'  '$@'"

tests: $(TESTS_OBJ)
	echo "Tests: $(TESTS)"
	echo "Test Objects: $(TESTS_OBJ)"


.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
