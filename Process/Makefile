#Makefile for process task
# make - скомпилировать
# make test - сгенерировать тесты и запустить тестировщик на всех тестах
# make lcov-test - make test под lcov
# make valgrind - make test под valgrind
# 
# 
# 
TARGET = tester

TEST_EXEC_D = ./tests/exec/
TEST_TXT_D = ./tests/txt/
SRCDIR = ./src/
INCDIR = ./headers/
OBJDIR = ./obj/

CC = gcc
CFLAGS = -O2 -Wall -Werror -pedantic-errors -Wno-pointer-sign -Wextra -std=gnu11

SOURCES = $(shell ls $(SRCDIR))
OBJECTS = $(SOURCES:.c=.o)

CRYPT = ./generator/crypt
CRYPT_C = ./generator/src/crypt.c


.PHONY: all lcov-test test valgrind clean crypt generate reset
all: $(TARGET)
$(TARGET): $(OBJECTS:%=$(OBJDIR)%)
	$(CC) -o $@ $(CFLAGS) $(OBJECTS)

$(OBJDIR)%.o: $(SRCDIR)%.c
	$(CC) -o $@ -c $(CFLAGS) -I $(INCDIR) $<

test: $(TARGET) generate
	./($TARGET) $(TEST_EXEC_D)*
generate:
	$(CRYPT) $(TEST_TXT_D)*
	$(CC) $(CFLAGS) -o $(TEST_TXT_D)*.out $(TEST_TXT_D)*.c
	rm -f $(TEST_TXT_D)*.c
output:
	mkdir output
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
	valgrind --leak-check=full --leak-resolution=med -v ./$(TARGET) 
	tail -n 15 output/valg_out
	echo "More info in output/valg_out"
	make clean
crypt: generate
	$(CC) -o $(CRYPT) $(CRYPT_C)
clean:
	rm -rf *.gc?? tests.info *.dSYM
reset: clean
	rm -rf $(OBJDIR)* html $(TARGET) output 