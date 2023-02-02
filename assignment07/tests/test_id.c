// SPDX-License-Identifier: GPL-3.0+
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "test_utils.h"

void test_id(void)
{
	int fd;
	int error;
	int ret;
	char buf[100];
	char *intra = "mypark";

	fd = start_test("/sys/kernel/debug/fortytwo/id", O_RDWR);

	ret = write(fd, intra, strlen(intra));
	print_result_write(ret);

	intra = "no_name";
	ret = write(fd, intra, strlen(intra));
	print_result_write(ret);

	ret = read(fd, buf, 3);
	print_result_read(ret, buf);

	ret = read(fd, buf, 6);
	print_result_read(ret, buf);

	ret = read(fd, buf, 10);
	print_result_read(ret, buf);

	end_test(fd);
}

