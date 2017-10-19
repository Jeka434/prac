#include <string.h>
#include <stdlib.h>
#include "First_task.h"

List * /* LIST ELEMENT INITIALIZATION */
init_elem(const char *added_line, List *prev, List *next)
{
    List *elem = calloc(1, sizeof(*elem));
//LCOV_EXCL_START
    if (elem == NULL) {
        /* MEMORY ERROR */
        return NULL;
    }
//LCOV_EXCL_STOP
    int line_size = strlen(added_line) + 1;
    elem->str = calloc(1, line_size * sizeof(*elem->str));
//LCOV_EXCL_START
    if (elem->str == NULL) {
        /* MEMORY ERROR */
        free(elem);
        return NULL;
    }
//LCOV_EXCL_STOP
    memcpy(elem->str, added_line, line_size);
    if(prev == NULL) {
        elem->prev = elem;
        elem->next = elem;
    } else {
        elem->prev = prev;
        elem->next = next;
    }
    return elem;
}

List * /* ADD ELEMENT TO LIST */
add_to_list(List *head_of_list, const char *added_line)
{
    if (added_line == NULL) {
        /* NO STRING TO ADD */
        return head_of_list;
    }
    if (head_of_list == NULL) {
        head_of_list = init_elem(added_line, NULL, NULL);
        return head_of_list;
    }
    List *list_pointer = head_of_list;
    while (list_pointer->next != head_of_list && strcmp(list_pointer->str, added_line) < 0) {
        list_pointer = list_pointer->next;
    }
    List *new_elem = NULL;
    if (strcmp(head_of_list->str, added_line) > 0 || strcmp(list_pointer->str, added_line) < 0) {
        list_pointer = head_of_list;
    }
    new_elem = init_elem(added_line, list_pointer->prev, list_pointer);
//LCOV_EXCL_START
    if (new_elem == NULL) {
        /* MEMORY ERROR */
        return NULL;
    }
//LCOV_EXCL_STOP
    list_pointer->prev->next = new_elem;
    list_pointer->prev = new_elem;
    if (strcmp(head_of_list->str, added_line) > 0) {
        head_of_list = new_elem;
    }
    return head_of_list;
}
