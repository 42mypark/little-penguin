#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

void print_result(int ret, const char *tag, char *buf) {
	if (ret < 0) 
		buf[0] = 0;
	else
		buf[ret] = 0;

	printf("%s\n", tag);
	printf("\tsize: %d\n",ret);
	printf("\tcontent: %s\n", buf);
	printf("\tmsg: %s\n", strerror(errno));

	errno = 0;
}

void test_sinario1(const char *filename) {
	int fd;
	int error;
	int size;
	char buf[100];


	printf("\n*****TEST: %s*****\n", filename);
	fd = open(filename, O_RDWR);
	if (fd < 0)
		err(1, "open failed");
	printf("open called: fd: %d\n", fd);

	size = read(fd, buf, 10);	
	print_result(size, "[read]", buf);

	size = read(fd, buf, 7);
	print_result(size, "[read]", buf);

	size = write(fd, buf, 6);
	print_result(size, "[write]", buf);

	size = write(fd, "asd", 3);
	print_result(size, "[write]", buf);

	error = close(fd);
	if (error < 0)
		err(1, "close failed");
	printf("close called\n");
}

void test_sinario2(const char *filename) {
	int fd;
	int error;
	int w_size;
	int r_size;
	char w_buf[100] = {0};
	char r_buf[100] = {0};

	memset(w_buf, 'a', 10);
	memset(w_buf + 10, 'b', 10);



	printf("\n*****TEST: %s*****\n", filename);
	fd = open(filename, O_RDWR);
	if (fd < 0)
		err(1, "open failed");
	printf("open called: fd: %d\n", fd);

	w_size += write(fd, w_buf, 6);
	print_result(w_size, "[write]", w_buf);
	w_buf[w_size] = 'a';

	w_size += write(fd, w_buf + w_size, 10);
	print_result(w_size, "[write]", w_buf);

	r_size = read(fd, r_buf, 12);
	print_result(r_size, "[read]", r_buf);

	r_size = read(fd, r_buf, 4);
	print_result(r_size, "[read]", r_buf);

	error = close(fd);
	if (error < 0)
		err(1, "close failed");
	printf("close called\n");
}


int main() {
	char *files[] = {
		"/sys/kernel/debug/fortytwo/id",
		"/sys/kernel/debug/fortytwo/jiffies",
		"/sys/kernel/debug/fortytwo/foo",
	};

	for (int i = 0; i < 2 ; i++) {
		test_sinario1(files[i]);
	}
	
	test_sinario2(files[2]);
}
