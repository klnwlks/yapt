CFLAGS += -lncurses -Wall -O3 -Wextra -lm
CC = gcc
SOURCE = main.c
EXECUTABLE = yapt

all: $(SOURCE) $(EXECUTABLE)

yapt: main.c config.h Makefile
	$(CC) $< -o $@ $(CFLAGS)

