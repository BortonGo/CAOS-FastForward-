CFLAGS = -g -O2 -Wall -Werror -std=c++23 -I $(NEJUDGE_DIR)/include
NEJUDGE_DIR ?= ../../../nejudge

nejudge-update:
	cd $(NEJUDGE_DIR) && ./update.bash

solution : main.S solution.S simpleio_x86_64.S
	$(CC) main.S solution.S simpleio_x86_64.S -o $@
	
test: solution nejudge-update
	python3 $(NEJUDGE_DIR)/test.py --user ejexec
	-rm solution
