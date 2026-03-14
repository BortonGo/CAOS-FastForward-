SOLUTION_SRC = solution.o main.o wrap_x86_64.o

include ../../../nejudge/Makefile.config

solution.o: solution.S
main.o: main.c
wrap_x86_64.o: ./wrap_x86_64.S
	$(CC) -DFUNC=bsearch40 -c $< -o $@

clean:
	-rm solution solution.o main.o wrap_x86_64.o
