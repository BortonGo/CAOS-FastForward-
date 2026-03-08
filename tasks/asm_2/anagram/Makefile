CFLAGS = -g -O2 -Wall -Werror -std=c++23 -I $(NEJUDGE_DIR)/include
NEJUDGE_DIR ?= ../../../nejudge

solution : solution.S
	$(CC) solution.S -o $@

test: solution
	@set -a; [ -f config.env ] && . ./config.env; set +a; python3 $(NEJUDGE_DIR)/test.py --user ejexec
	-rm solution
