#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("MYPARK");
MODULE_ALIAS("input:b000*v0627p0001*");

int init_my_keyboard(void) {
	pr_info("Hello keyboard ! \n");
	return 0;
}

void cleanup_my_keyboard(void) {
	pr_info("Cleanning up keyboard.\n");
}

module_init(init_my_keyboard);
module_exit(cleanup_my_keyboard);

