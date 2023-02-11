// SPDX-License-Identifier: GPL-3.0+
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "test_utils.h"

#define PAGE_SIZE (1 << 12)

static void print_result_open(int fd, char *flag)
{
	static int c;

	printf("#%d: %s\n", c++, flag);
	printf("\tfd: %d\n", fd);
	printf("\tmsg: %s\n", strerror(errno));
	if (fd >= 0)
		close(fd);
	errno = 0;
}

void test_permission(void)
{
	int fd, c;

	printf("\n*****Test foo: Permission*****\n");

	setuid(1000);

	fd = open("/sys/kernel/debug/fortytwo/foo", O_RDONLY);
	print_result_open(fd, "O_RDONLY");
	fd = open("/sys/kernel/debug/fortytwo/foo", O_WRONLY);
	print_result_open(fd, "O_WRONLY");
	fd = open("/sys/kernel/debug/fortytwo/foo", O_RDWR);
	print_result_open(fd, "O_RDWR");
	fd = open("/sys/kernel/debug/fortytwo/foo", O_APPEND);
	print_result_open(fd, "O_APPEND");
	fd = open("/sys/kernel/debug/fortytwo/foo", O_CREAT);
	print_result_open(fd, "O_CREAT");
	fd = open("/sys/kernel/debug/fortytwo/foo", O_ASYNC);
	print_result_open(fd, "O_ASYNC");
	fd = open("/sys/kernel/debug/fortytwo/foo", O_NONBLOCK);
	print_result_open(fd, "O_NONBLOCK");
	fd = open("/sys/kernel/debug/fortytwo/foo", O_DIRECTORY);
	print_result_open(fd, "O_DIRECTORY");
	fd = open("/sys/kernel/debug/fortytwo/foo", O_EXCL);
	print_result_open(fd, "O_EXCL");

	exit(0);
}

void test_write(void)
{
	int fd, ret, c;
	char buf[100] = {0};

	printf("\n*****Test foo: Write*****\n");
	read(1, &c, 1);

	fd = open("/sys/kernel/debug/fortytwo/foo", O_RDWR);
	printf("\nfd: %d\n", fd);
	ret = write(fd, "0123456789", 10);
	print_result_write(ret);

	ret = read(fd, buf, 10);
	print_result_read(ret, buf);

	close(fd);
}

void test_read(void)
{
	int fd, ret, c;
	char buf[100] = {0};

	printf("\n*****Test foo: Read*****\n");
	read(1, &c, 1);

	fd = open("/sys/kernel/debug/fortytwo/foo", O_RDWR);

	ret = read(fd, buf, 4);
	print_result_read(ret, buf);
	ret = read(fd, buf, 4);
	print_result_read(ret, buf);
	ret = read(fd, buf, 4);
	print_result_read(ret, buf);
	ret = read(fd, buf, 4);
	print_result_read(ret, buf);

	close(fd);
}

void test_nospec(void)
{
	int fd, ret, c;
	char buf[PAGE_SIZE] = {'a'};

	printf("\n*****Test foo: NOSPEC*****\n");
	read(1, &c, 1);

	fd = open("/sys/kernel/debug/fortytwo/foo", O_RDWR);

	ret = write(fd, buf, PAGE_SIZE);
	print_result_write(ret);

	ret = write(fd, buf, 1);
	print_result_write(ret);

	ret = write(fd, buf, 0);
	print_result_write(ret);

	close(fd);
}

void *child(void *arg)
{
	int fd, ret;
	char buf[10];

	fd = open("/sys/kernel/debug/fortytwo/foo", O_RDWR);

	for (int i = 0; i < (PAGE_SIZE / 4); i++)
		ret = read(fd, buf, 4);

	close(fd);
	return NULL;
}

void test_spinlock(void)
{
	int c;

	printf("\n*****Test foo: spinlock*****\n");
	read(1, &c, 1);

	pthread_t tids[10];

	for (int i = 0; i < 10; i++)
		pthread_create(&tids[i], NULL, child, NULL);

	for (int i = 0; i < 10; i++)
		pthread_join(tids[i], NULL);

	printf("\n*****Test foo: spinlock DONE*****\n");
}

void test_foo(void)
{
	int ws;
	int pid;

	printf("\n*****Test foo*****\n");
	read(1, &ws, 1);

	pid = fork();
	if (pid == 0)
		test_permission();
	else
		wait(&ws);
	test_write();
	test_read();
	test_nospec();
	test_spinlock();
}
