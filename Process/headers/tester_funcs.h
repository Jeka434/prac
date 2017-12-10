#ifndef _TESTER_FUNCS_H_
#define _TESTER_FUNCS_H_

#include <sys/types.h>

pid_t testpid;
int pp1[2], pp0[2];

enum {
    BUFSIZE = 513,
    NUMSIZE = 20
};

int qnum;

void close_all(void);
void handler(int signum);
int gettopic1(void);
int getq1(int qnum);
int send1(int qnum, int stats[]);
void print_stats(int stats[], int size);

#endif
