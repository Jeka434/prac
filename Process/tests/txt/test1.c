#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

enum {
    BUFSIZE = 513
};

char const *topic = "Тест 1";

const char *questions[][2] = {
    {"Сколько будет 120+23?", "143"},
    {"Напишите \"привет\".", "привет"},
    {"Это третий вопрос? (да/нет)", "да"},
    {"Кто сдаст экзамен по ОС?", "никто"},
    {"экзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзаменэкзамен", "фыв"}
};

char str[BUFSIZE];

#define check_err(func) if ((func) < 0) goto err;

#define check(cmd) switch (cmd) { \
    case -1: return -1; \
}

int qnum = sizeof(questions) / sizeof(*questions);

int getqnum(void)
{
    return write(1, &qnum, sizeof(qnum));
}

int gettopic(void)
{
    return printf("Topic is %s\n", topic);
}

int getq(void)
{
    int qnum;
    if (scanf("%d%*c", &qnum) < 0) {
        return -1;
    }
    return printf("%d. %s\n", qnum, questions[qnum - 1][0]);
}

int check_answer(int qnum, const char *msg)
{
    if (!strcmp(msg, questions[qnum - 1][1])) {
        return printf("1");
    } else {
        return printf("0");
    }
}

int send(void)
{
    int qnum;
    if (scanf("%d%*c", &qnum) < 0) {
        return 1;
    }
    if (!fgets(str, BUFSIZE, stdin)) {
        return 1;
    }

    if (strchr(str, '\n')) {
        strchr(str, '\n')[0] = 0;
    }
    return check_answer(qnum, str);
}

int (*cmds[])(void) = {
    getqnum,
    gettopic,
    getq,
    send
};

int main(int argc, char const *argv[])
{
    setbuf(stdout, NULL);
    int cmd;
    while ((cmd = getchar()) != EOF) {
        if (cmd >= '0' && cmd <= '3') {
            check_err(cmds[cmd - '0']());
        }
    }
    return 0;
err:
    perror(argv[argc - 1]);
    return 1;
}
