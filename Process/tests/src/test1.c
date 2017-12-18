#include <string.h>

enum {
    BUFSIZE = 513
};

struct Question {
    char *q;
    char *a;
    int (*cmp)(const char *, const char *);
};

char const *topic = "Тест 1";

struct Question questions[] = {
    {"Сколько будет 120+23?", "143", strcmp},
    {"Напишите \"привет\".", "привет", strcmp},
    {"Это третий вопрос? (да/нет)", "да", strcmp},
    {"Кто сдаст экзамен по ОС?", "никто", strcmp}
};

#include ".main_test.c"
