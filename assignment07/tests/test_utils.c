#include <stdio.h>
#include <errno.h>
#include <err.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int start_test(const char *filename, int flags) {
	int fd;

	printf("\n*****TEST: %s*****\n", filename);
	fd = open(filename, flags);
	if (fd < 0)
		err(1, "open failed");
	return fd;
}

void end_test(int fd) {
	int error;

	error = close(fd);
	if (error < -1)
		err(0, "close failed");
	printf("close called\n");
}

void print_result_read(int ret, char *buf) {
	if (ret < 0) 
		buf[0] = 0;
	else
		buf[ret] = 0;

	printf("[read]\n");
	printf("\tsize: %d\n",ret);
	printf("\tcontent: %s\n", buf);
	printf("\tmsg: %s\n", strerror(errno));

	errno = 0;
}

void print_result_write(int ret) {
	printf("[write]\n");
	printf("\tsize: %d\n", ret);
	printf("\tmsg: %s\n", strerror(errno));

	errno = 0;
}
