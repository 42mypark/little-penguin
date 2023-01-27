#ifndef TEST_UTILS_H
#define TEST_UTILS_H

void print_result_read(int ret, char *buf);
int start_test(const char *filename, int flags);
void end_test(int fd);
void print_result_write(int ret);

#endif


