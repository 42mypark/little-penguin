// SPDX-License-Identifier: GPL-3.0+

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");

void do_work(int my_int, int *retval)
{
	int i;
	int y = my_int;

	for (i = 0; i < my_int; ++i)
		mdelay(10);

	if (y > 10) {
		/* That was a long sleep, tell userspace about it */
		pr_info("We slept a long time!");
	}
	*retval = i * y;
}

int my_init(void)
{
	int x = 10;

	do_work(x, &x);
	return x;
}

void my_exit(void)
{
}

module_init(my_init);
module_exit(my_exit);
