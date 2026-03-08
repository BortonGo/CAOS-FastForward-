.PHONY: test clean

NEJUDGE_DIR ?= ../../../nejudge

solution: solution.o main.o wrap_x86_64.o

solution.o: solution.S
main.o: main.c
wrap_x86_64.o: ./wrap_x86_64.S
	$(CC) -DFUNC=bsearch40 -c $< -o $@

test: solution
	python3 $(NEJUDGE_DIR)/test.py --user ejexec
	@rm -f solution solution.o main.o wrap_x86_64.o
