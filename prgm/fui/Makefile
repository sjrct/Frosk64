#
# Makefile for fui
#
# written by sjrct
#

SHELL   = /bin/sh

TARGET  = fui

SRC_1   = fui.c

CC      = gcc

SRC     = $(addprefix src/,$(SRC_1))
TMP     = $(SRC:.c=.o)
OBJ     = $(TMP:src/%=obj/%)

all: $(TARGET)

$(TARGET): obj $(OBJ)
	$(cmd) "$(CC) $(FROSK_LDFL) -o $(TARGET) $(OBJ) $(LIBS_FULL)" "CC $(TARGET)"

obj:
	$(cmd) "mkdir obj" "MKDIR obj"

obj/%.o: src/%.c
	$(cmd) "$(CC) -c $(FROSK_CCFL) -o $@ $<" "CC $@"

.PHONY: clean
clean:
	-$(cmd) "$(RM) $(TARGET)" "RM $(TARGET)"
	-$(cmd) "$(RM) -rf obj" "RM obj"

