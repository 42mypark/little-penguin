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


int main(int argc, char **argv) {
	int fd;
	int error;
	int size;
	char buf[100];

	if (argc != 2) {
		printf("check arguments!\n");
		return 1;
	}

	fd = open(argv[1], O_RDWR);
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
