// SPDX-License-Identifier: GPL-3.0+
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

void ft_close(int fd)
{
	int error;

	error = close(fd);
	if (error < 0)
		err(1, "close failed");
	printf("close called\n\n");
}

int ft_open(void)
{
	int fd;

	fd = open("/dev/fortytwo", O_RDWR);
	if (fd < 0)
		err(1, "open failed");
	printf("open called: fd: %d\n", fd);
	return fd;
}

int main(void)
{
	int fd;
	int error;
	int size;
	char buf[100];

	{
		fd = ft_open();

		size = read(fd, buf, 6);
		buf[size] = 0;
		printf("read called\n");
		printf("\tsize: %d\n", size);
		printf("\tcontent: %s\n", buf);

		ft_close(fd);
	}

	{
		fd = ft_open();

		size = write(fd, buf, 6);
		printf("write called\n");
		printf("\tsize: %d\n", size);
		printf("\tmsg: %s\n", strerror(errno));

		ft_close(fd);
	}

	{
		fd = ft_open();

		size = write(fd, "asd", 3);
		printf("write called\n");
		printf("\tsize: %d\n", size);
		printf("\tmsg: %s\n", strerror(errno));

		ft_close(fd);
	}

	{
		fd = ft_open();

		size = write(fd, "asdasdasd", 9);
		printf("write called\n");
		printf("\tsize: %d\n", size);
		printf("\tmsg: %s\n", strerror(errno));

		ft_close(fd);
	}
}
