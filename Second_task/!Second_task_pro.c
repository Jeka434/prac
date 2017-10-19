#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int process_file(char const *file_name);

int main(int argc, char const *argv[])
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

int process_file(char const *file_name)
{
    FILE *fd = fopen(file_name, "r+");
    char str[10000];
    long size;
        printf("ftell=%ld\n", ftell(fd));
    char asd;
    int num;
    while ((num = fscanf(fd, "%[^\n]%1[\n]%ln", str, &asd, &size)) != EOF) {
        if (num) {
            printf("%s : size=%ld", str, size);
            printf("%c", asd);
        } else {
            printf("\n");
            fseek(fd, 1, SEEK_CUR);
        }
    }
    /*
    off_t file_size = ftell(fd);
    if(truncate(file_name, file_size) < 0) {
        //TRUNCATE FILE ERROR 
        goto error_end;
    }
    */
    fclose(fd);
    return 0;
error_end:
    fclose(fd);
    return -1;
}