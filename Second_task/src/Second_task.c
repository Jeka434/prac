#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "Second_task_funcs.h"

int
main(int argc, char const *argv[])
{
    if(argc == 1) {
        fprintf(stderr, "Sorry, no file to process.\n");
    }
    for (int i = 1; i < argc; i++) {
        if (process_file(argv[i]) < 0) {
            fprintf(stderr, "Error in file %s : %s\n", argv[i], strerror(errno));
        }
    }
    return 0;
}
