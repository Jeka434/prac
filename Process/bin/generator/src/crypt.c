#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum {
    KEY = 69,

    ADDEDLEN = 10,
    STRLEN = 2048,
};

int qnum;

const char *head1 = "#include <stdio.h>\n"
"#include <string.h>\n"
"#include <ctype.h>\n"
"#include <unistd.h>\n"
"\n"
"enum {\n"
"    BUFSIZE = 513,\n"
"    KEY = ";
const char *head2 = ",\n"
"    QUESTNUM = ";
const char *body1 = "\n"
"};\n"
"\n"
"char const *topic = \"";
const char *body2 = "\";\n"
"";
const char *body3 = "\n"
"char *questions[QUESTNUM][2] = {";
const char *tail = "};\n"
"char str[BUFSIZE];\n"
"\n"
"#define check_err(func) if ((func) < 0) goto err;\n"
"\n"
"int qnum = QUESTNUM;\n"
"\n"
"int getqnum(void)\n"
"{\n"
"    return write(1, &qnum, sizeof(qnum));\n"
"}\n"
"\n"
"int gettopic(void)\n"
"{\n"
"    return printf(\"Topic is %s\\n\", topic);\n"
"}\n"
"\n"
"int getq(void)\n"
"{\n"
"    int qnum;\n"
"\n"
"    if (scanf(\"%d%*c\", &qnum) < 0) {\n"
"        return -1;\n"
"    }\n"
"    return printf(\"%d. %s\\n\", qnum, questions[qnum - 1][0]);\n"
"}\n"
"\n"
"int check_answer(int qnum, const char *msg)\n"
"{\n"
"    if (!strcmp(msg, questions[qnum - 1][1])) {\n"
"        return printf(\"1\");\n"
"    } else {\n"
"        return printf(\"0\");\n"
"    }\n"
"}\n"
"\n"
"int send(void)\n"
"{\n"
"    int qnum;\n"
"    if (scanf(\"%d%*c\", &qnum) < 0) {\n"
"        return -1;\n"
"    }\n"
"    if (!fgets(str, BUFSIZE, stdin)) {\n"
"        return -1;\n"
"    }\n"
"    strchr(str, '\\n')[0] = 0;\n"
"    return check_answer(qnum, str);\n"
"}\n"
"\n"
"int (*cmds[])(void) = {\n"
"    getqnum,\n"
"    gettopic,\n"
"    getq,\n"
"    send\n"
"};\n"
"\n"
"void decrypt(char *str)\n"
"{\n"
"    while (*str != KEY) {\n"
"        *str++ ^= KEY;\n"
"    }\n"
"    *str = 0;\n"
"}\n"
"\n"
"void decrypt_all(void)\n"
"{\n"
"    int i;\n"
"    for(i = 0; i < qnum; i++) {\n"
"        decrypt(questions[i][0]);\n"
"        decrypt(questions[i][1]);\n"
"    }\n"
"}\n"
"\n"
"int main(int argc, char const *argv[])\n"
"{\n"
"    setbuf(stdout, NULL);\n"
"    decrypt_all();\n"
"    int cmd;\n"
"    while ((cmd = getchar()) != EOF) {\n"
"        if (cmd >= '1' && cmd <= '3') {\n"
"            check_err(cmds[cmd - '0']());\n"
"        }\n"
"    }\n"
"    return 0;\n"
"err:\n"
"    perror(argv[argc - 1]);\n"
"    return 1;\n"
"}\n";

int generate_cfile(FILE *dst, FILE *src)
{
    fprintf(dst, "%s%d%s%d%s", head1, KEY, head2, qnum, body1);
    int c, qi = 0, qora = 0, i;
    int topflag = 1, queflag = 0;
    while ((c = fgetc(src)) != EOF) {
        if (queflag) {
            if (qora) {
                fprintf(dst, "char q%d[] = {", qi);
            } else {
                fprintf(dst, "char a%d[] = {", qi);
            }
            queflag = 0;
        }
        if (c != '\n') {
            if (topflag) {
                fputc(c, dst);
            } else {
                fprintf(dst, "%u, ", (unsigned char)c ^ KEY);
            }
        } else {
            if ((c = fgetc(src)) == '#') {
                if ((c = fgetc(src)) == '$') {
                    if ((c = fgetc(src)) == '\n') {
                        if (topflag) {
                            topflag = 0;
                            fprintf(dst, "%s", body2);
                        } else {
                            fprintf(dst, "%d};\n", KEY);
                        }
                        if (!qora) {
                            qi++;
                        }
                        qora = !qora;
                        queflag = 1;
                    } else {
                        fprintf(dst, "\n#$%c", c);
                    }
                } else {
                    fprintf(dst, "\n#%c", c);
                }
            } else {
                fprintf(dst, "\n%c", c);
            }
        }
    }
    fprintf(dst, "%s", body3);
    for (i = 1; i < qi - 1; i++) {
        fprintf(dst, "{q%d, a%d},", i, i);
    }
    fprintf(dst, "{q%d, a%d}%s", i, i, tail);
    return 0;
}

int analize(const char *name)
{
    FILE *fd = fopen(name, "r");
    if (!fd) perror(NULL);
    char c;
    int count = 0, flag = 0;
    while ((c = fgetc(fd)) != EOF) {
        if (c == '\n' || c == '#' || c == '$') {
            if (flag == 0 && c == '\n') {
                flag = 1;
            } else if (flag == 1 && c == '#') {
                flag = 2;
            } else if (flag == 2 && c == '$') {
                flag = 3;
            } else if (flag == 3 && c == '\n') {
                flag = 0;
                count++;
            }
        } else {
            flag = 0;
        }
    }
    fclose(fd);
    if (!(count % 2)) {
        fprintf(stderr, "%d\n", count);
        return -1;
    }
    return (count - 1) / 2;
}

int main(int argc, char const *argv[])
{
    char *name;
    FILE *source;
    FILE *dest;
    int len;
    for (int i = 1; i < argc; ++i) {
        if ((qnum = analize(argv[i])) < 0 ) {
            fprintf(stderr, "%s: Wrong file\n", argv[i]);
            continue;
        }
        if (!(source = fopen(argv[i], "r"))) {
            perror(argv[i]);
            continue;
        }
        len = strlen(argv[i]);
        name = calloc(len + ADDEDLEN + 2, sizeof(char));
        memmove(name, argv[i], len);
        memmove(&name[len], ".c", 3);
        dest = fopen(name, "w");
        free(name);
        generate_cfile(dest, source);
        fclose(source);
        fclose(dest);
    }
    return 0;
}