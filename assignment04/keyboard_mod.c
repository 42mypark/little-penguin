#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

// modalias input:b0003v0627p0001e0001-e0,1,11,14,k71,72,73,74,75,77,79,7A,7B,7C,7D,7E,7F,80,81,82,83,84,85,86,87,88,89,8A,8B,8C,8E,8F,9B,9C,9D,9E,9F,A3,A4,A5,A6,AC,AD,E2,raml0,1,2,sfw

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

