# Makefile for the first task
NAME = first_program

TESTDIR = ./tests/
INCDIR = ./headers/
SRCDIR = ./src/
OBJDIR = ./obj/
BINDIR = ./bin/
TARGET = $(BINDIR)$(NAME)
CC = gcc
TESTFILES = $(shell ls $(TESTDIR)file*)
SOURCES = $(shell ls $(SRCDIR))
OBJECTS = $(SOURCES:.c=.o)
CFLAGS = -O2 -Wall -Werror -pedantic-errors -Wno-pointer-sign -Wextra -std=gnu11 -I $(INCDIR) -fsanitize=undefined -ftrapv
LFLAGS = -Wall -Werror -pedantic-errors -Wno-pointer-sign -Wextra -std=gnu11 -I $(INCDIR) --coverage

.PHONY: all lcov-test test valgrind clean reset getready

all: $(TARGET)
$(TARGET): $(OBJECTS:%=$(OBJDIR)%)
	$(CC) -o $@ $(CFLAGS) $(OBJECTS:%=$(OBJDIR)%)

$(OBJDIR)%.o: $(SRCDIR)%.c
	$(CC) -o $@ -c $(CFLAGS) -I $(INCDIR) $<

output:
	mkdir output
test: $(TARGET) output
	./$(TARGET) 1>output/stdout 2>output/stderr
	./$(TARGET) $(TESTFILES) 1>>output/stdout 2>>output/stderr
lcov:
	git clone https://github.com/linux-test-project/lcov.git
lcov-test: lcov reset
	make test CFLAGS="$(LFLAGS)"
	lcov/bin/lcov --capture --directory ./ --output-file tests.info --rc lcov_branch_coverage=1
	lcov/bin/genhtml tests.info --branch-coverage -o html
	make clean
valgrind: output reset
	make CFLAGS="$(CFLAGS) -g"
	valgrind --leak-check=full --leak-resolution=med -v ./$(TARGET) $(TESTFILES) 1>output/valg_out 2>&1
	tail -n 15 output/valg_out
	echo "More info in output/valg_out"
	make clean
stderr:
	less output/stderr
stdout:
	less output/stdout
clean:
	rm -rf $(OBJDIR)*.gc?? tests.info $(BINDIR)*.dSYM 
reset: clean
	rm -rf html $(OBJDIR)* $(TARGET) output/* $(TESTFILES)
	cp tests/copied/* tests/
getready: reset
	rm -rf lcov output
