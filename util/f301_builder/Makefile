#
# f301_builder: Makefile
#
# written by sjrct
#

SHELL   = /bin/sh
CC      = gcc
CCFLAGS = -c -Wall
LDFLAGS = 
TARGET  = f301_builder

TSRCS   = main.c fs.c parse.c error.c
SRCS    = $(TSRCS:%=src/%)

TOBJS   = $(SRCS:.c=.o)
OBJS    = $(TOBJS:src/%=obj/%)


.PHONY: all
all: $(SRCS) $(TARGET)

$(TARGET): obj $(OBJS)
	$(cmd) "$(CC) $(OBJS) $(LDFLAGS) -o $@" "CC $@"

obj:
	$(cmd) "mkdir obj" "MKDIR obj"

obj/%.o: src/%.c
	$(cmd) "$(CC) $(CCFLAGS) $< -o $@" "CC $@"

.PHONY: clean
clean:
	-$(cmd) "$(RM) $(TARGET)" "RM $(TARGET)"
	-$(cmd) "$(RM) -rf obj" "RM obj"
