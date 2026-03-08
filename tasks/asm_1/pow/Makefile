CFLAGS = -g -O2 -Wall -Werror -std=c++23 -I $(NEJUDGE_DIR)/include
NEJUDGE_DIR ?= ../../../nejudge

solution : solution.S
	$(CC) solution.S simpleio_x86_64.S -o $@

test: solution
	python3 $(NEJUDGE_DIR)/test.py --user ejexec
	-rm solution
	
docker-test:
	docker run --rm -it -v "$(realpath ../../../)":/app -w /app/tasks/asm_1/pow asm-ubuntu make test
