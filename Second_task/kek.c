#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    long size;//kkk
    FILE *fd = fopen(argv[1], "r");
    char c;
    fscanf(fd, "%*[^\n]%ln%c", &size, &c);
    return 0;
}