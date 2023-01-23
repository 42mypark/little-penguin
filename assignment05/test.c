#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>


int main() {
	int fd;
	int error;
	int size;
	char buf[100];

	fd = open("/dev/fortytwo", O_RDWR);
	if (fd < 0)
		err(1, "open failed");
	printf("open called: fd: %d\n", fd);

	size = read(fd, buf, 6);
	buf[size] = 0;
	printf("read called\n");
	printf("\tsize: %d\n",size);
	printf("\tcontent: %s\n", buf);

	size = write(fd, buf, 6);
	printf("write called\n");
	printf("\tsize: %d\n",size);

	size = write(fd, "asd", 3);
	printf("write called\n");
	printf("\tsize: %d\n",size);
	printf("\tmsg: %s\n", strerror(errno));

	error = close(fd);
	if (error < 0)
		err(1, "close failed");
	printf("close called\n");
		
}
