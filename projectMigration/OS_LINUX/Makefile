CC      := gcc
CFLAGS  := -Wall -Wextra -O2
LDLIBS  := -lm

BIN_DIR := bin

TARGETS := example0 example1 example2
COMMON_OBJ := $(BIN_DIR)/tinyexpr.o
TARGET_BINS := $(addprefix $(BIN_DIR)/, $(TARGETS))

all: $(BIN_DIR) $(TARGET_BINS)

# Ensure bin directory exists
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Link executables
$(BIN_DIR)/example0: $(BIN_DIR)/example0.o $(COMMON_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

$(BIN_DIR)/example1: $(BIN_DIR)/example1.o $(COMMON_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

$(BIN_DIR)/example2: $(BIN_DIR)/example2.o $(COMMON_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# Compile object files into bin/
$(BIN_DIR)/%.o: %.c tinyexpr.h | $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean
