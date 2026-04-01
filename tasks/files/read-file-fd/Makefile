CFLAGS = -O2 -g -std=gnu11 -Wl,--wrap=__read_chk -Wl,--wrap=read -Wl,--wrap=malloc -Wl,--wrap=calloc -Wl,--wrap=reallocarray -Wl,--wrap=realloc
SOLUTION_SRC = solution.c main.c sha256.c
NEJUDGE_FLAGS = --valgrind

include ../../../nejudge/Makefile.config
