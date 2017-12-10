#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../headers/tester_funcs.h"


#define check(cmd) if ((cmd) < 0) return -1;

pid_t testpid;
int pp1[2], pp0[2];

void close_all(void)
{
    close(pp0[0]);
    close(pp0[1]);
    close(pp1[0]);
    close(pp1[1]);
}

void handler()
{
    close_all();
    wait(NULL);
    exit(1);
}

char buf[BUFSIZE];

int print_from_pipe(int fd)
{
    int count;
    while ((count = read(fd, buf, BUFSIZE)) == BUFSIZE && buf[BUFSIZE - 1] != '\n') {
        check(write(1, buf, BUFSIZE));
    }
    check(count);
    check(write(1, buf, count));
    return 0;
}

int writeint(int fd, int qnum)
{
    char str[NUMSIZE];
    int len = snprintf(str, NUMSIZE, "%d ", qnum);
    return write(fd, str, len);
}

int getqnum1(void)
{
    check(write(pp1[1], "0", 1));
    return read(pp0[0], &qnum, sizeof qnum);
}

int gettopic1(void)
{
    check(write(pp1[1], "1", 1));
    return print_from_pipe(pp0[0]);
}

int getq1(int qnum)
{
    check(write(pp1[1], "2", 1));
    check(writeint(pp1[1], qnum));
    return print_from_pipe(pp0[0]);
}

int send1(int qnum, int stats[])
{
    check(write(pp1[1], "3", 1));
    check(writeint(pp1[1], qnum));
    if (!fgets(buf, BUFSIZE, stdin)) {
        return -1;
    }
    check(write(pp1[1], buf, strlen(buf)));
    char res;
    check(read(pp0[0], &res, sizeof(res)));
    if (res - '0') {
        stats[qnum - 1] = 1;
        return 1;
    } else {
        stats[qnum - 1] = 0;
        return 0;
    }
}

void print_stats(int stats[], int size)
{
    int i, summ = 0;
    for (i = 0; i < size; i++) {
        summ += stats[i];
    }
    printf("\nCorrect answers are %d from %d\n\nDetails:\n", summ, size);
    for (i = 0; i < size; i++) {
        printf("Question %d == %s\n", i + 1, stats[i] ? "CORRECT" : "INCORRECT");
    }
}
