#
# Makefile for cstd
#
# written by sjrct
#

SHELL   = /bin/sh

TARGET  = cstd.a

CSRCS_B = string.c ctype.c rand.c time.c malloc.c

CC      = gcc
AR      = ar

CSRCS = $(addprefix src/,$(CSRCS_B))
COBJS = $(addprefix obj/,$(CSRCS_B:.c=.o))

.PHONY: all
all: obj $(TARGET)

obj:
	$(cmd) "mkdir obj" "MKDIR obj"

$(TARGET): $(COBJS) $(AOBJS)
	$(cmd) "$(AR) rcu $@ $^" "AR $(TARGET)"

obj/%.o: src/%.c
	$(cmd) "$(CC) -c $< $(FROSK_CCFL) -o $@" "CC $@"

.PHONY: clean
clean:
	-$(cmd) "$(RM) $(TARGET)" "RM $(TARGET)"
	-$(cmd) "$(RM) -rf obj" "RM obj"
