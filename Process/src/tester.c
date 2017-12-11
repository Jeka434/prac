#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include "tester_funcs.h"

int statflag = 0, qi = 0;

#define main_check(cmd) \
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
    exit(0);
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
//LCOV_EXCL_START
    main_check(testpid);
//LCOV_EXCL_STOP
    setsignals();
//LCOV_EXCL_START
    main_check(getqnum1());
//LCOV_EXCL_STOP
    memset(stats, 0, qi);
    statflag = 1;

//LCOV_EXCL_START
    main_check(gettopic1());
//LCOV_EXCL_STOP
    int res;
    for (qi = 1; qi < qnum + 1; qi++) {
//LCOV_EXCL_START
        main_check(getq1(qi));
//LCOV_EXCL_STOP
        res = send1(qi, stats);
        main_check(res);
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
