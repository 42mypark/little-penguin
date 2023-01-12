#include <linux/module.h>
#include <linux/kernel.h>

int init_module(void) {
	printk(KERN_INFO "Hello world !\n");
	return 0;
}

int cleanup_module(void) {
	printk("Cleanning up module.\n");
}

module_init(init_module);
module_exit(cleanup_module);

