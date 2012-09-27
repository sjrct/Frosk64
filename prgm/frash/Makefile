#
# Makefile for frash
#
# written by sjrct
#

SHELL   = /bin/sh

TARGET  = frash

SRC_1   = frash.c

CC      = gcc

SRC     = $(addprefix src/,$(SRC_1))
TMP     = $(SRC:.c=.o)
OBJ     = $(TMP:src/%=obj/%)

all: $(TARGET)

$(TARGET): obj $(OBJ)
	$(cmd) "$(CC) -o $(TARGET) $(OBJ) $(FROSK_LDFL)" "CC $(TARGET)"

obj:
	$(cmd) "mkdir obj" "MKDIR obj"

obj/%.o: src/%.c
	$(cmd) "$(CC) $(FROSK_CCFL) -o $@ $<" "CC $@"

.PHONY: clean
clean:
	-$(cmd) "$(RM) $(TARGET)" "RM $(TARGET)"
	-$(cmd) "$(RM) -rf obj" "RM obj"

