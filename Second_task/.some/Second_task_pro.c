#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

enum {
    BUF_SIZE = 512,
    MAX_NUMBER_SIZE = 32, /* SIZE OF BUFFER FOR STRING-NUMBERS */
};

int process_file(char const *file_name);

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

int
process_file(char const *file_name)
{
    int read_fd = open(file_name, O_RDONLY);
    if (read_fd == -1) {
        /* OPEN RDONLY FILE ERROR */
        return -1;
    }
    int write_fd = open(file_name, O_RDWR);
    if (write_fd == -1) {
        /* OPEN RDWR FILE ERROR */
        close(read_fd);
        return -1;
    }
    lseek(write_fd, 0, SEEK_SET);
    char rd_buf[BUF_SIZE];
    char opt_buf[BUF_SIZE];
    char number_char, summ_char;
    int act_size, i, opt_size_rd, opt_size_wr;
    //long long number = 0, summ = 0, numlen;
    int number_flag = 0, summ_exists = 0, file_end = 0, negative_flag = 1;
    off_t line_start = 0, cur_pos = 0, file_str_len, opt_size_sum;
    off_t number_start = 0, number_end, summ_end, summ_size;
    while ((act_size = read(read_fd, rd_buf, BUF_SIZE)) > 0) {
        cur_pos = lseek(read_fd, 0, SEEK_CUR);
        if (cur_pos == lseek(read_fd, 0, SEEK_END)) {
            file_end = 1;
        }
        lseek(read_fd, cur_pos, SEEK_SET);
        summ_end = line_start;
        for (i = 0; i < act_size; i++) {
            if (rd_buf[i] == '-') {
                negative_flag = -1;
            } else if (rd_buf[i] >= '0' && rd_buf[i] <= '9') {
                if (!number_flag) {
                    number_flag = 1;
                    number_start = lseek(rd_buf, 0, SEEK_CUR) - act_size + i;
                }
            } else {
                if (number_flag) {
                    number_flag = 0;
                    number_end = lseek(read_fd, 0, SEEK_CUR) - act_size + i;
                    if (!summ_exists) {
                        /* Запись числа в начало строки */
                        lseek(read_fd, 0, number_start);
                        opt_size_sum = 0;
                        number_len = number_end - number_start + 1;
                        summ_size = number_len;
                        while ((opt_size = read(read_fd, opt_buf, BUF_SIZE)) > 0) {
                            opt_size_sum += opt_size;
                            if (opt_size_sum < number_len) {
                                if (write(write_fd, opt_buf, opt_size) < 0) {
                                    /* WRITE FILE ERROR */
                                    goto error_end;
                                }
                            } else {
                                if (write(write_fd, opt_buf,
                                        number_len - opt_size_sum + opt_size) < 0) {
                                    /* WRITE FILE ERROR */
                                    goto error_end;
                                }
                                break;
                            }
                        }
                        if (opt_size_rd < 0) {
                            /* READ FILE ERROR */
                            goto error_end;
                        }
                        summ_exists = 1;
                    } else {
                        /* Суммирование числа в начало сроки */
                        number_len = number_end - number_start + 1;
                        lseek(read_fd, 1, SEEK_CUR);
                        while(lseek(read_fd, -2, SEEK_CUR) )

                    }
                }
                if (rd_buf[i] == '\n' || (file_end && i == act_size - 1)) {
                    if (!summ_exists) {

                    }
                    summ_exists = 0;
                }
            }
        }
    }
    if (act_size < 0) {
        /* READ FILE ERROR */
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
