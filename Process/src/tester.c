#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include "../headers/tester_funcs.h"

#define main_check(cmd) if ((cmd) < 0) kill(getpid(), SIGINT);

int main(int argc, char const *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Неправильное количество аргументов\n");
        return 1;
    }
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
    if (testpid == -1) {
        return 1;
    }
    signal(SIGINT, handler);
    signal(SIGCHLD, handler);
    main_check(read(pp0[0], &qnum, sizeof(qnum)));
    int stats[qnum];
    memset(stats, 0, qnum);

    main_check(gettopic1());
    int i, res;
    for (i = 1; i < qnum + 1; i++) {
        main_check(getq1(i));
        res = send1(i, stats);
        main_check(res);
        if (res) {
            printf("CORRECT\n");
        } else {
            printf("INCORRECT\n");
        }
    }
    print_stats(stats, qnum);
    close_all();
    wait(NULL);
    return 0;
}
