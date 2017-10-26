#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "First_task.h"
//LCOV_EXCL_START
enum {
    STR_SIZE = 1024,
    FILE_ERROR = -1,
    MEMORY_ERROR = -2
};

int delete_list(List *head_of_list)
{
    if (!head_of_list) {
        return 0;
    }
    List *tmp;
    head_of_list->prev->next = NULL;
    while (head_of_list) {
        tmp = head_of_list;
        head_of_list = head_of_list->next;
        free(tmp->str);
        free(tmp);
    }
    return 0;
}

void print_list(const List *head_of_list)
{
    printf("\n--- list_start ---\n");
    if (!head_of_list) {
        printf("empty list");
        printf("\n--- list_end --- \n");
        return;
    }
    const List *end = head_of_list;
    do {
        printf("/%s", head_of_list->str);
        head_of_list = head_of_list->next;
    } while (head_of_list != end);
    printf("/\n--- list_end --- \n");
}

int create_list_by_test(List **head_of_list, const char *filename)
{
    FILE *fd = fopen(filename, "r");
    if (!fd) {
        return FILE_ERROR;
    }
    print_list(*head_of_list);
    List *tmp_list;
    char str[STR_SIZE];
    while (fgets(str, STR_SIZE, fd)) {
        if (str[strlen(str) - 1] == '\n') {
            str[strlen(str) - 1] = 0;
        }
        if (!(tmp_list = add_to_list(*head_of_list, str))) {
            fclose(fd);
            return MEMORY_ERROR;
        }
        *head_of_list = tmp_list;
        print_list(*head_of_list);
    }
    fclose(fd);
    return 0;
}

int main(int argc, char const *argv[])
{
    printf("START\n");
    List *my_list = NULL;
    for (int i = 1; i < argc; ++i)
    {
        printf("Start creating list by testfile '%s'\n", argv[i]);
        switch (create_list_by_test(&my_list, argv[i])) {
            case FILE_ERROR:
                perror(argv[i]);
                break;
            case MEMORY_ERROR:
                delete_list(my_list);
                perror(argv[i]);
                break;
        }
        add_to_list(my_list, NULL);
        delete_list(my_list);
        my_list = NULL;
        printf("Stop creating list by testfile '%s'\n", argv[i]);
    }
    printf("END\n");
    return 0;
}
//LCOV_EXCL_STOP
