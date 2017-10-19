# Makefile for the first task
TESTFILES = file file2 file3
TESTDIR = ./tests/
INCDIR = ./includes/
SRCDIR = ./src/
TARGET = first_program
CC = gcc
SOURCES = First_task_test.c First_task.c
OBJECTS = $(SOURCES:.c=.o)
LIB = First_task.h
CFLAGS = -O2 -g -Wall -Werror --coverage -pedantic-errors -Wno-pointer-sign -Wextra -std=gnu11 -ftrapv -I $(INCDIR)
# -fsanitize=undefined

all: $(TARGET)
$(TARGET): $(OBJECTS)
	$(CC) -o $@ $(CFLAGS) $^
.c.o:
	$(CC) -c $(CFLAGS) $(SRCDIR)$<
$(SOURCES): $(INCDIR)$(LIB)
mactest: $(TARGET)
	mkdir output
	./$(TARGET) $(patsubst %, $(TESTDIR)%, $(TESTFILES)) 1>output/stdout
	lcov --capture --directory ./ --output-file tests.info --rc lcov_branch_coverage=1
	genhtml tests.info --branch-coverage -o html
bsdtest: $(TARGET)
	mkdir output
	./$(TARGET) $(patsubst %, $(TESTDIR)%, $(TESTFILES)) 1>output/stdout
	lcov/bin/lcov --capture --directory ./ --output-file tests.info --rc lcov_branch_coverage=1
	lcov/bin/genhtml tests.info --branch-coverage -o html
	echo Press any button to continue with valgrind...
	read char
	valgrind --leak-check=full --leak-resolution=med -v ./$(TARGET) $(patsubst %, $(TESTDIR)%, $(TESTFILES)) 1>output/valg_stdout
clean:
	rm -rf html *.gc?? tests.info $(TARGET) *.o output