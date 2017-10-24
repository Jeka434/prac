#include <stdio.h>
#include "Second_task_funcs.h"

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
    MAX_NUMBER_SIZE = 32, /* SIZE OF BUFFER FOR STRING-NUMBERS */
};

/* GET STRING FROM INTEGER */
int int_to_str(char *str, int num)
{
    int i = 0;
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
    while (num) {
        str[i++] = '0' + num % 10;
        num /= 10;
    }
    char tmp;
    for (int j = 0; j < i / 2; j++) {
        tmp = str[j + neg];
        str[j + neg] = str[i - j - 1 + neg];
        str[i - j - 1 + neg] = tmp;
    }
    str[i + neg] = '\0';
    return i + neg;
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
        if (ferror(read_fd)) {
            /* READING FILE ERROR */
            goto error_end;
        }
        cur_pos = ftell(read_fd);
        fseek(read_fd, 0, SEEK_END);
        if (cur_pos == ftell(read_fd)) {
            file_end = 1;
        }
        fseek(read_fd, cur_pos, SEEK_SET);
        for (i = 0; i < act_size; i++) {
            if (buf[i] >= '0' && buf[i] <= '9') {
                number_flag = 1;
                number = number * 10 + buf[i] - '0';
                if (number < 0) {
                    /* TOO BIG NUMBER */
                }
            }
            if (!(buf[i] >= '0' && buf[i] <= '9') || (file_end && i == act_size - 1)) {
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
                    if (fwrite(str_number, sizeof(*str_number), numlen * sizeof(*str_number),
                        write_fd) != numlen * sizeof(*str_number)) {
                        /* WRITING FILE ERROR */
                        goto error_end;
                    }
                    summ_exists = 0;
                    summ = 0;
                } else {
                    fseek(read_fd, line_start, SEEK_SET);
                    opt_size_sum = 0;
                    file_str_len = cur_pos - act_size + i - line_start + (buf[i] != '\n');
                    while ((opt_size = fread(opt_buf,
                        sizeof(*opt_buf), sizeof(opt_buf) / sizeof(*opt_buf), read_fd))) {
                        if (ferror(read_fd)) {
                            /* READING ERROR */
                            goto error_end;
                        }
                        opt_size_sum += opt_size;
                        if (opt_size_sum < file_str_len) {
                            if (fwrite(opt_buf, sizeof(*opt_buf), opt_size, write_fd) != opt_size) {
                                /* WRITING FILE ERROR */
                                goto error_end;
                            }
                        } else {
                            fwrite(opt_buf, sizeof(*opt_buf),
                                file_str_len - opt_size_sum + opt_size, write_fd);
                            break;
                        }
                    }
                    if (ferror(read_fd)) {
                        /* READING FILE ERROR */
                        goto error_end;
                    }
                    fseek(read_fd, cur_pos, SEEK_SET);
                }
                line_start = cur_pos - act_size + i + 1;
                if (buf[i] == '\n' && fwrite("\n", 1, 1, write_fd) != 1) {
                    /* WRITING FILE ERROR */
                    goto error_end;
                }
            }
        }
    }
    if (ferror(read_fd)) {
        /* READING FILE ERROR */
        goto error_end;
    }
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
