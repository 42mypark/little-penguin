#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include "id.h"
#include "jiffies.h"
#include "foo.h"

#define FT_DIRNAME "fortytwo"

MODULE_LICENSE("GPL");

struct dentry *fortytwo_dir = NULL;

int init_learn_debugfs(void) {
	int error;

	pr_info("Hello learn_debugfs !\n");	

	fortytwo_dir = debugfs_create_dir(FT_DIRNAME, NULL);
	if (!fortytwo_dir){
		pr_info("debugfs_create_dir: %s: failed", FT_DIRNAME);
		return -EAGAIN;
	}

	error = id_init();
	if (error)
		return error;

	error = jiffies_init();
	if (error)
		return error;

	error = foo_init();
	if (error)
		return error;
	
	return 0;
}

void cleanup_learn_debugfs(void) {
	foo_exit();
	debugfs_remove_recursive(fortytwo_dir);
	pr_info("Cleanning up learn_debugfs.\n");
}

module_init(init_learn_debugfs);
module_exit(cleanup_learn_debugfs);


