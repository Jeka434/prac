#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include "../headers/tester_funcs.h"

int statflag = 0, qi = 0;

#define MAIN_CHECK(cmd) \
switch (cmd) { \
    case -1: handler(); \
}

void setsignals(void);

void handler()
{
    setsignals();
    if (statflag) {
        print_stats(stats, qi - 1, qnum);
    }
    close_all();
    wait(NULL);
    _exit(0);
}

void setsignals(void)
{
    signal(SIGINT, handler);
    signal(SIGPIPE, handler);
}

int main(int argc, char const *argv[])
{
//LCOV_EXCL_START
    if (argc != 2) {
        fprintf(stderr, "Неправильное количество аргументов\n");
        return 0;
    }
//LCOV_EXCL_STOP
    pipe(pp0);
    pipe(pp1);
    if (!(testpid = fork())) {
        dup2(pp1[0], 0);
        dup2(pp0[1], 1);
        close_all();
        execl(argv[1], argv[1], (char*)0);
        perror(argv[1]);
        return 1;
    }
    close(pp0[1]);
    close(pp1[0]);
    MAIN_CHECK(testpid);
    setsignals();
    MAIN_CHECK(getqnum1());
    memset(stats, 0, qi);
    statflag = 1;

    MAIN_CHECK(gettopic1());
    int res;
    for (qi = 1; qi < qnum + 1; qi++) {
        MAIN_CHECK(getq1(qi));
        res = send1(qi, stats);
        MAIN_CHECK(res);
        if (res) {
            printf("CORRECT\n");
        } else {
            printf("INCORRECT\n");
        }
    }
    print_stats(stats, qnum, qnum);
    close_all();
    wait(NULL);
    return 0;
}
