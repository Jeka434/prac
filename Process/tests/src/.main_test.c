#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

char str[BUFSIZE];

#define CHECK_ERR(func) if ((func) < 0) goto err;

#define CHECK(cmd) switch (cmd) { \
    case -1: return -1; \
}

int qnum = sizeof(questions) / sizeof(*questions);

int getqnum(void)
{
    return write(1, &qnum, sizeof(qnum));
}

int gettopic(void)
{
    int len = strlen(topic) + strlen("Topic is \n");
    CHECK(write(1, &len, sizeof len));
    return printf("Topic is %s\n", topic);
}

int getq(void)
{
    int qnum1;
    CHECK(scanf("%d%*c", &qnum1));
    if (qnum1 > qnum || qnum1 < 0) {
        return -1;
    }
    int len = strlen(questions[qnum1 - 1].q) + 1;
    CHECK(write(1, &len, sizeof len));
    return printf("%s\n", questions[qnum1 - 1].q);
}

int check_answer(int qnum, const char *msg)
{
    if (!questions[qnum - 1].cmp(msg, questions[qnum - 1].a)) {
        return printf("1");
    } else {
        return printf("0");
    }
}

int send(void)
{
    int qnum1;
    CHECK(scanf("%d%*c", &qnum1));
    if (qnum1 > qnum || qnum1 < 0) {
        return -1;
    }
    if (!fgets(str, BUFSIZE, stdin)) {
        return -1;
    }

    if (strchr(str, '\n')) {
        strchr(str, '\n')[0] = 0;
    }
    return check_answer(qnum1, str);
}

int (*cmds[])(void) = {
    getqnum,
    gettopic,
    getq,
    send
};

int main(void)
{
    setbuf(stdout, NULL);
    int cmd;
    while ((cmd = getchar()) != EOF) {
        if (cmd >= '0' && cmd <= '3') {
            CHECK_ERR(cmds[cmd - '0']());
        }
    }
    return 0;
err:
    return 1;
}
