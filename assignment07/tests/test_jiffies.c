// SPDX-License-Identifier: GPL-3.0+
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "test_utils.h"

void test_jiffies(void)
{
	int fd = start_test("/sys/kernel/debug/fortytwo/jiffies", O_RDWR);
	int ret;
	char buf[100] = {0};

	ret = read(fd, NULL, 10);
	print_result_read(ret, buf);

	ret = read(fd, buf, 3);
	print_result_read(ret, buf);

	ret = read(fd, buf, 10);
	print_result_read(ret, buf);

	ret = read(fd, buf, 100);
	print_result_read(ret, buf);

	end_test(fd);
}
