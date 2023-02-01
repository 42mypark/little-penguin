// SPDX-License-Identifier: GPL-3.0+
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");

int init_module(void)
{
	pr_info("Hello world !\n");
	return 0;
}

void cleanup_module(void)
{
	pr_info("Cleanning up module.\n");
}

//module_init(init_module);
//module_exit(cleanup_module);

