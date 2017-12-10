#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

enum {
    BUFSIZE = 513,
    KEY = 69,
    QUESTNUM = 3
};

char const *topic = "Тест 1";
char q1[] = {149, 228, 149, 255, 149, 251, 149, 254, 148, 201, 149, 255, 149, 251, 101, 149, 244, 148, 198, 149, 241, 149, 240, 148, 199, 101, 116, 119, 117, 110, 119, 118, 122, 69};
char a1[] = {116, 113, 118, 69};
char q2[] = {149, 216, 149, 245, 149, 250, 149, 253, 148, 205, 149, 253, 148, 199, 149, 240, 101, 103, 149, 250, 148, 197, 149, 253, 149, 247, 149, 240, 148, 199, 103, 107, 69};
char a2[] = {149, 250, 148, 197, 149, 253, 149, 247, 149, 240, 148, 199, 69};
char q3[] = {149, 232, 148, 199, 149, 251, 101, 148, 199, 148, 197, 149, 240, 148, 199, 149, 253, 149, 252, 101, 149, 247, 149, 251, 149, 250, 148, 197, 149, 251, 148, 196, 122, 101, 109, 149, 241, 149, 245, 106, 149, 248, 149, 240, 148, 199, 108, 69};
char a3[] = {149, 241, 149, 245, 69};

char *questions[QUESTNUM][2] = {{q1, a1},{q2, a2},{q3, a3}};
char str[BUFSIZE];

#define check_err(func) if ((func) < 0) goto err;

int qnum = QUESTNUM;

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
        return -1;
    }
    if (!fgets(str, BUFSIZE, stdin)) {
        return -1;
    }
    strchr(str, '\n')[0] = 0;
    return check_answer(qnum, str);
}

int (*cmds[])(void) = {
    getqnum,
    gettopic,
    getq,
    send
};

void decrypt(char *str)
{
    while (*str != KEY) {
        *str++ ^= KEY;
    }
    *str = 0;
}

void decrypt_all(void)
{
    int i;
    for(i = 0; i < qnum; i++) {
        decrypt(questions[i][0]);
        decrypt(questions[i][1]);
    }
}

int main(int argc, char const *argv[])
{
    setbuf(stdout, NULL);
    decrypt_all();
    int cmd;
    while ((cmd = getchar()) != EOF) {
        if (cmd >= '0' && cmd <= '3') {
            check_err(cmds[cmd - '0']());
        }
    }
    return 0;
err:
    perror(argv[0]);
    return 1;
}
