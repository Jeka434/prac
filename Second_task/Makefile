# Makefile for the first task
TESTFILES = file1 file2 file3 file4 file5 file6
TESTDIR = ./tests/
INCDIR = ./includes/
SRCDIR = ./src/
TARGET = second_program
CC = gcc
SOURCES = Second_task.c Second_task_funcs.c
OBJECTS = $(SOURCES:.c=.o)
LIB = Second_task_funcs.h
CFLAGS = -O2 -g -Wall -Werror --coverage -pedantic-errors -Wno-pointer-sign -Wextra -std=gnu11 -ftrapv -I $(INCDIR) -fsanitize=undefined

all: $(TARGET)
$(TARGET): $(OBJECTS)
	$(CC) -o $@ $(CFLAGS) $^
.c.o:
	$(CC) -c $(CFLAGS) $(SRCDIR)$<
$(SOURCES): $(INCDIR)$(LIB)
output:
	mkdir output
test: $(TARGET) output
	./$(TARGET) $(patsubst %, $(TESTDIR)%, $(TESTFILES)) 1>output/stdout 2>output/stderr
lcov: test
	lcov --capture --directory ./ --output-file tests.info --rc lcov_branch_coverage=1
	genhtml tests.info --branch-coverage -o html
dlcov: test
	lcov/bin/lcov --capture --directory ./ --output-file tests.info --rc lcov_branch_coverage=1
	lcov/bin/genhtml tests.info --branch-coverage -o html
valgrind: output
	rm -f tests/file*
	cp tests/copied/* tests/
	valgrind --leak-check=full --leak-resolution=med -v ./$(TARGET) $(patsubst %, $(TESTDIR)%, $(TESTFILES)) 1>output/valg_stdout 2>output/valg_stderr

clean:
	rm -rf html *.gc?? tests.info $(TARGET) *.o output tests/file*
	cp tests/copied/* tests/