#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("MYPARK");

int init_module(void) {
	printk(KERN_INFO "Hello world !\n");
	return 0;
}

void cleanup_module(void) {
	printk("Cleanning up module.\n");
}

//module_init(init_module);
//module_exit(cleanup_module);

