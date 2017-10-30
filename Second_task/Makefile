# Makefile for the second task
TARGET = second_program
SOURCES = Second_task.c Second_task_funcs.c
LIB = Second_task_funcs.h
TESTFILES = file1 file2 file3 file4 file5 file6 file7 file8 file9 file10
TESTDIR = ./tests/
INCDIR = ./includes/
SRCDIR = ./src/
CC = gcc
OBJECTS = $(SOURCES:.c=.o)
CFLAGS = -O2 -Wall -Werror -pedantic-errors -Wno-pointer-sign -Wextra -std=gnu11 -I $(INCDIR)
TFLAGS = -fsanitize=undefined -ftrapv
TRAPFLAG = 0

all: $(TARGET)
$(TARGET): $(OBJECTS)
ifeq ($(TRAPFLAG),0)
	$(CC) -o $@ $(CFLAGS) $^
else
	$(CC) -o $@ $(CFLAGS) $(TFLAGS) $^
endif
	rm -rf *.o
.c.o:
ifeq ($(TRAPFLAG),0)
	$(CC) -c $(CFLAGS) $(SRCDIR)$<
else
	$(CC) -c $(CFLAGS) $(TFLAGS) $(SRCDIR)$<
endif
$(SOURCES): $(INCDIR)$(LIB)
output:
	mkdir output
test: $(TARGET) output
	./$(TARGET) 1>output/stdout 2>output/stderr
	chmod -r tests/file8
	chmod -w tests/file9
	./$(TARGET) $(patsubst %, $(TESTDIR)%, $(TESTFILES)) 1>>output/stdout 2>>output/stderr
lcov:
	git clone https://github.com/linux-test-project/lcov.git
lcov-test: lcov
	make test CFLAGS="$(CFLAGS) --coverage"
	lcov/bin/lcov --capture --directory ./ --output-file tests.info --rc lcov_branch_coverage=1
	lcov/bin/genhtml tests.info --branch-coverage -o html
	make clean
	open html/index.html
valgrind: output
	make CFLAGS="$(CFLAGS) -g"
	valgrind --leak-check=full --leak-resolution=med -v ./$(TARGET) $(patsubst %, $(TESTDIR)%, $(TESTFILES)) 1>output/valg_out 2>&1
	tail -n 15 output/valg_out
	echo "More info in output/valg_out"
	make clean
stderr:
	cat output/stderr
stdout:
	cat output/stdout
clean:
	rm -rf *.gc?? tests.info *.dSYM 
reset:
	chmod +r tests/file8 || true
	chmod +w tests/file9 || true
	rm -rf html *.gc?? tests.info $(TARGET) output tests/file* *.dSYM
	cp tests/copied/* tests/
lcov-rm:
	rm -rf lcov
