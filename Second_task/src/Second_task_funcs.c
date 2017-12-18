#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "../headers/Second_task_funcs.h"

#if defined(__APPLE__) || defined(unix) || defined(__unix) || defined(__unix__)
#include <unistd.h>
#define trun(file, size) ftruncate(file, size)
#elif (defined _WIN32) || (defined _WIN64)
#define trun(file, size) _ftruncate(file, size)
#else
#error Wrong system
#endif

enum {
    BUF_SIZE = 512,
    MAX_NUMBER_SIZE = 32 /* SIZE OF BUFFER FOR STRING-NUMBERS */
};

#define CHECK_ERR(file) \
if (ferror(file)) {\
    goto error_end;\
}

/* GET STRING FROM INTEGER */
int int_to_str(char *str, int num)
{
    if (!num) {
        str[0] = '0';
        str[1] = '\0';
        return 1;
    }
    int neg = 0;
    if (num < 0) {
        neg = 1;
        num = -num;
        str[0] = '-';
    }
    int i = 0;
    while (num) {
        str[i++ + neg] = '0' + num % 10;
        num /= 10;
    }
    char tmp;
    for (int j = neg; j < neg + i / 2; j++) {
        tmp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = tmp;
    }
    str[i + neg] = '\0';
    return i + neg;
}

int check_for_end(FILE *file, long cur_pos)
{
    fseek(file, 0, SEEK_END);
    if (cur_pos == ftell(file)) {
        fseek(file, cur_pos, SEEK_SET);
        return 1;
    }
    fseek(file, cur_pos, SEEK_SET);
    return 0;
}

/* PROCESSING ONE FILE */
int process_file(char const *file_name)
{
    FILE *read_fd = fopen(file_name, "rb");
    if (!read_fd) {
        /* OPEN RD FILE ERROR */
        return -1;
    }
    FILE *write_fd = fopen(file_name, "r+b");
    if (!write_fd) {
        /* OPEN WR FILE ERROR */
        fclose(read_fd);
        return -1;
    }
    fseek(write_fd, 0, SEEK_SET);
    char buf[BUF_SIZE];
    char opt_buf[BUF_SIZE];
    char str_number[MAX_NUMBER_SIZE] = {0};
    size_t act_size, i, opt_size;
    long long number = 0, summ = 0, numlen;
    int number_flag = 0, summ_exists = 0, file_end = 0, negative_flag = 1;
    long line_start = 0, cur_pos = 0, file_str_len, opt_size_sum;
    while ((act_size = fread(buf, sizeof(*buf), sizeof(buf) / sizeof(*buf), read_fd))) {
        CHECK_ERR(read_fd);
        file_end = check_for_end(read_fd, cur_pos = ftell(read_fd));
        for (i = 0; i < act_size; i++) {
            if (isdigit(buf[i])) {
                number_flag = 1;
                number = number * 10 + buf[i] - '0';
            }
            if (!isdigit(buf[i]) || (file_end && i == act_size - 1)) {
                if (number_flag) {
                    number_flag = 0;
                    summ_exists = 1;
                    summ += number * negative_flag;
                    number = 0;
                }
                if (buf[i] != '-') {
                    negative_flag = 1;
                }
            }
            if (buf[i] == '-') {
                negative_flag = -1;
            }
            if (buf[i] == '\n' || (file_end && i == act_size - 1)) {
                if (summ_exists) {
                    numlen = int_to_str(str_number, summ);
                    fwrite(str_number, sizeof(*str_number), numlen * sizeof(*str_number), write_fd);
                    CHECK_ERR(write_fd);
                    summ_exists = 0;
                    summ = 0;
                } else {
                    fseek(read_fd, line_start, SEEK_SET);
                    opt_size_sum = 0;
                    file_str_len = cur_pos - act_size + i - line_start + (buf[i] != '\n' ? 1 : 0);
                    while ((opt_size = fread(opt_buf,
                        sizeof(*opt_buf), sizeof(opt_buf) / sizeof(*opt_buf), read_fd))) {
                        CHECK_ERR(read_fd);
                        opt_size_sum += opt_size;
                        if (opt_size_sum < file_str_len) {
                            fwrite(opt_buf, sizeof(*opt_buf), opt_size, write_fd);
                            CHECK_ERR(write_fd);
                        } else {
                            fwrite(opt_buf, sizeof(*opt_buf),
                                file_str_len - opt_size_sum + opt_size, write_fd);
                            CHECK_ERR(write_fd);
                            break;
                        }
                    }
                    CHECK_ERR(read_fd);
                    fseek(read_fd, cur_pos, SEEK_SET);
                }
                line_start = cur_pos - act_size + i + 1;
                if (buf[i] == '\n') {
                    fwrite("\n", 1, 1, write_fd);
                    CHECK_ERR(write_fd);
                }
            }
        }
    }
    CHECK_ERR(read_fd);
    fflush(write_fd);
    if(trun(fileno(write_fd), ftell(write_fd)) < 0) {
        /* TRUNCATE FILE ERROR */
        goto error_end;
    }
    fclose(read_fd);
    fclose(write_fd);
    return 0;
error_end:
    fclose(read_fd);
    fclose(write_fd);
    return -1;
}
