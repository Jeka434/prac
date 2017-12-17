#Makefile for process task
# make - скомпилировать
# make test - сгенерировать тесты и запустить тестировщик на всех тестах
# make lcov-test - make test под lcov
# make valgrind - make test под valgrind
# make clean - очистить рабочую область
# make reset - вернуться в начало
# make generate - скомпилировать тесты
NAME = tester

TEST_EXEC_D = ./tests/exec/
TEST_TXT_D = ./tests/txt/
SRCDIR = ./src/
INCDIR = ./headers/
OBJDIR = ./obj/
BINDIR = ./bin/
TARGET = $(BINDIR)$(NAME)

CC = gcc
CFLAGS = -O2 -Wall -Werror -pedantic-errors -Wno-pointer-sign -Wextra -std=gnu11 -fsanitize=undefined -ftrapv

LFLAGS = -Wall -Werror -pedantic-errors -Wno-pointer-sign -Wextra -std=gnu11 --coverage

SOURCES = $(shell ls $(SRCDIR))
OBJECTS = $(SOURCES:.c=.o)

.PHONY: all lcov-test test valgrind clean reset getready

all: $(TARGET)
$(TARGET): $(OBJECTS:%=$(OBJDIR)%)
	$(CC) -o $@ $(CFLAGS) $(OBJECTS:%=$(OBJDIR)%)

$(OBJDIR)%.o: $(SRCDIR)%.c
	$(CC) -o $@ -c $(CFLAGS) -I $(INCDIR) $<

test: $(TARGET) output generate
	for name in $(TEST_EXEC_D)*.out; do \
	./$(TARGET) $$name; \
	done
generate: $(TEST_EXEC_D)
	rm -f $(TEST_EXEC_D)*
	for name in `ls $(TEST_TXT_D)*.c | awk -F '/' '{print $$NF;}'`; do \
	$(CC) $(CFLAGS) -o $(TEST_EXEC_D)$$name.out $(TEST_TXT_D)$$name; \
	done
$(TEST_EXEC_D):
	mkdir $(TEST_EXEC_D)
output:
	mkdir output
lcov:
	git clone https://github.com/linux-test-project/lcov.git
lcov-test: lcov reset
	make test CFLAGS="$(LFLAGS)"
	lcov/bin/lcov --capture --directory ./ --output-file tests.info --rc lcov_branch_coverage=1
	lcov/bin/genhtml tests.info --branch-coverage -o html
	make clean
	echo "open html/index.html"
valgrind: output reset
	make CFLAGS="$(CFLAGS) -g"
	for name in $(TEST_EXEC_D)*.out; do \
	valgrind --leak-check=full --leak-resolution=med -v ./$(TARGET) $$name 2>>output/valg_out; \
	done
	echo "More info in output/valg_out"
	make clean
clean:
	rm -rf $(OBJDIR)*.gc?? tests.info $(BINDIR)*.dSYM
reset: clean
	rm -rf $(OBJDIR)* $(TARGET) output html $(BINDIR)*
getready: reset
	rm -rf lcov $(TEST_EXEC_D)*