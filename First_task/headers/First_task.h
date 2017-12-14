#ifndef FIRSTTASKH
#define FIRSTTASKH value
typedef struct list
{
    char *str;
    struct list *next;
    struct list *prev;
} List;

List * /* ADD ELEMENT TO LIST */
add_to_list(List *head_of_list, const char *added_line);
#endif
