CC = gcc
CFLAGS = -Wall -Wextra -MMD -MP

SRC_DIR = src
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj

BIN = $(BUILD_DIR)/a.out
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS = $(OBJS:.o=.d)

.PHONY: all clean debug

all: $(BIN)

debug: CFLAGS += -g3
debug: all

$(BIN): $(OBJS) | $(BUILD_DIR)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	rm -r $(OBJ_DIR) $(BUILD_DIR)

-include $(DEPS)