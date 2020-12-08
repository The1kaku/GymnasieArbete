CC = gcc
TARGET_EXEC ?= random.exe
LDFLAGS :=
BUILD_DIR ?= ./build
SRC_DIRS ?= ./src
CPPFLAGS ?= $(INC_FLAGS) -MMD -MP -Wall -g

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)
	-@echo "***************** Done ******************"

# c source
$(BUILD_DIR)/%.c.o: %.c
	-@$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) -r $(BUILD_DIR)

.PHONY: help
	-@echo "clean: cleanup build directory"
	-@echo ": make the game"

-include $(DEPS)

MKDIR_P ?= mkdir -p