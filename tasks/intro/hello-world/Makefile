.PHONY: test clean

NEJUDGE_DIR ?= ../../../nejudge

solution: solution.c

test: solution
	python3 $(NEJUDGE_DIR)/test.py --user ejexec
	@rm -f solution
