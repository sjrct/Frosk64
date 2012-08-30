#
# Makefile for start
#
# written by sjrct
#

SHELL   = /bin/sh

TARGET  = start

SRC_1   = start.c

CC      = gcc
CCFLAGS = -c -Wall -m64 -ffreestanding -mno-red-zone -mcmodel=large -nostdinc -O -fno-toplevel-reorder
LDFLAGS = -m64 -ffreestandings -mno-red-zone -mcmodel=large -nostdlib -T$(TOPDIR)/prgm/fbe.ld

SRC     = $(addprefix src/,$(SRC_1))
TMP     = $(SRC:.c=.o)
OBJ     = $(TMP:src/%=obj/%)

all: $(TARGET)

$(TARGET): obj $(OBJ)
	$(cmd) "$(CC) -o $(TARGET) $(OBJ) $(LDFLAGS)" "CC $(TARGET)"

obj:
	$(cmd) "mkdir obj" "MKDIR obj"

obj/%.o: src/%.c
	$(cmd) "$(CC) $(CCFLAGS) -o $@ $<" "CC $@"

.PHONY: clean
clean:
	-$(cmd) "$(RM) $(TARGET)" "RM $(TARGET)"
	-$(cmd) "$(RM) $(OBJ)" "RM obj/"
