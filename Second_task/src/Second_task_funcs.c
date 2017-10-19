#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

enum {
    BUF_SIZE = 512,
    MAX_NUMBER_SIZE = 32, /* SIZE OF BUFFER FOR STRING-NUMBERS */
};

int
int_to_str(char *str, int num)
{
    int i = 0;
    if(!num) {
        str[0] = '0';
        str[1] = '\0';
        return 1;
    }
    while (num) {
        str[i++] = '0' + num % 10;
        num /= 10;
    }
    char tmp;
    for (int j = 0; j < i / 2; j++) {
        tmp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = tmp;
    }
    str[i] = '\0';
    return i;
}

int
process_file(char const *file_name)
{
    int read_fd = open(file_name, O_RDONLY);
    if (read_fd == -1) {
        /* OPEN RD FILE ERROR */
        return -1;
    }
    int write_fd = open(file_name, O_WRONLY);
    if (write_fd == -1) {
        /* OPEN WR FILE ERROR */
        close(read_fd);
        return -1;
    }
    lseek(write_fd, 0, SEEK_SET);
    char buf[BUF_SIZE];
    char opt_buf[BUF_SIZE];
    char str_number[MAX_NUMBER_SIZE] = {0};
    int act_size, i, opt_size;
    long long number = 0, summ = 0, numlen;
    int number_flag = 0, summ_exists = 0, file_end = 0, negative_flag = 1;
    off_t line_start = 0, cur_pos = 0, file_str_len, opt_size_sum;
    while ((act_size = read(read_fd, buf, BUF_SIZE)) > 0) {
        cur_pos = lseek(read_fd, 0, SEEK_CUR);
        if (cur_pos == lseek(read_fd, 0, SEEK_END)) {
            file_end = 1;
        }
        lseek(read_fd, cur_pos, SEEK_SET);
        for (i = 0; i < act_size; i++) {
            if (buf[i] == '-') {
                negative_flag = -1;
            }
            if (buf[i] >= '0' && buf[i] <= '9') {
                number_flag = 1;
                number = number * 10 + buf[i] - '0';
                if (number < 0) {
                    /* TOO BIG NUMBER */
                }
            } else {
                if (number_flag) {
                    number_flag = 0;
                    summ_exists = 1;
                    summ += number * negative_flag;
                    number = 0;
                }
                negative_flag = 1;
            }
            if (buf[i] == '\n' || (file_end && i == act_size - 1)) {
                if (summ_exists) {
                    numlen = int_to_str(str_number, summ);
                    if (write(write_fd, str_number, numlen) < 0) {
                        /* WRITING FILE ERROR */
                        goto error_end;
                    }
                    summ_exists = 0;
                    summ = 0;
                } else {
                    lseek(read_fd, line_start + 1, SEEK_SET);
                    opt_size_sum = 0;
                    file_str_len = cur_pos - act_size + i - line_start - 1;
                    while ((opt_size = read(read_fd, opt_buf, BUF_SIZE)) > 0) {
                        opt_size_sum += opt_size;
                        if (opt_size_sum < file_str_len) {
                            if (write(write_fd, opt_buf, opt_size) < 0) {
                                goto error_end;
                            }
                        } else {
                            write(write_fd, opt_buf, file_str_len - opt_size_sum + opt_size);
                            break;
                        }
                    }
                    if (opt_size < 0) {
                        /* READING ERROR */
                        goto error_end;
                    }
                    lseek(read_fd, 0, cur_pos);
                }
                line_start = lseek(read_fd, 0, SEEK_CUR) - act_size + i;
                if (!(file_end && i == act_size - 1) && write(write_fd, "\n", 1) < 0) {
                    /* WRITING FILE ERROR */
                    goto error_end;
                }
            }
        }
    }
    if (act_size < 0) {
        /* READING FILE ERROR */
        goto error_end;
    }
    off_t file_size = lseek(write_fd, 0, SEEK_CUR);
    if(truncate(file_name, file_size) < 0) {
        /* TRUNCATE FILE ERROR */
        goto error_end;
    }
    close(read_fd);
    close(write_fd);
    return 0;
error_end:
    close(read_fd);
    close(write_fd);
    return -1;
}
