EXE := simple_ed

SRC_DIR := src
SRC := $(wildcard $(SRC_DIR)/*.c)

OBJ_DIR := obj
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CFLAGS := -Wall -Wextra -Weffc++ -Wsign-conversion -pedantic-errors
CPPFLAGS := -Iinclude

.PHONY: all clean debug

all: $(EXE)

debug: CFLAGS += -g3
debug: $(EXE)

$(EXE): $(OBJ)
	$(CC) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	$(RM) -rf $(OBJ_DIR) $(EXE)
