// SPDX-License-Identifier: GPL-3.0+
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "tests.h"

int main(void)
{
	test_id();
	test_jiffies();
	test_foo();
}
