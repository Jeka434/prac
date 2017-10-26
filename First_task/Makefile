# Makefile for the first task
TARGET = first_program
SOURCES = First_task.c First_task_test.c
LIB = First_task.h
TESTFILES = file1 file2 file3 file4
TESTDIR = ./tests/
INCDIR = ./includes/
SRCDIR = ./src/
CC = gcc
OBJECTS = $(SOURCES:.c=.o)
CFLAGS = -O2 -g -Wall -Werror --coverage -pedantic-errors -Wno-pointer-sign -Wextra -std=gnu11 -ftrapv -I $(INCDIR) -fsanitize=undefined
TRAPFLAG = 0

all: $(TARGET)
$(TARGET): $(OBJECTS)
ifeq ($(TRAPFLAG),0)
	$(CC) -o $@ $(CFLAGS) $^
else
	$(CC) -o $@ $(CFLAGS) -fsanitize=undefined -ftrapv $^
endif
.c.o:
ifeq ($(TRAPFLAG),0)
	$(CC) -c $(CFLAGS) $(SRCDIR)$<
else
	$(CC) -c $(CFLAGS) -fsanitize=undefined -ftrapv $(SRCDIR)$<
endif
$(SOURCES): $(INCDIR)$(LIB)
output:
	mkdir output
test: $(TARGET) output
	./$(TARGET) 1>output/stdout 2>output/stderr
	./$(TARGET) $(patsubst %, $(TESTDIR)%, $(TESTFILES)) 1>output/stdout 2>output/stderr
lcov: test
	lcov --capture --directory ./ --output-file tests.info --rc lcov_branch_coverage=1
	genhtml tests.info --branch-coverage -o html
dlcov: test
	lcov/bin/lcov --capture --directory ./ --output-file tests.info --rc lcov_branch_coverage=1
	lcov/bin/genhtml tests.info --branch-coverage -o html
valgrind: $(TARGET) output
	valgrind --leak-check=full --leak-resolution=med -v ./$(TARGET) $(patsubst %, $(TESTDIR)%, $(TESTFILES)) 2>&1 1>output/valg_out
	tail -n 15 output/valg_out
	echo "More info in output/valg_out"
stderr:
	cat output/stderr
stdout:
	cat output/stdout
clean:
	rm -rf html *.gc?? tests.info $(TARGET) *.o output tests/file* *.dSYM
	cp tests/copied/* tests/