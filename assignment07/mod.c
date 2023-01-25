#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include "id.h"
#include "jiffies.h"
#include "foo.h"

#define FT_DIRNAME "fortytwo"

MODULE_LICENSE("GPL");

static struct dentry* fortytwo_dir = NULL;

int init_learn_debugfs(void) {
	pr_info("Hello learn_debugfs !\n");	

	fortytwo_dir = debugfs_create_dir(FT_DIRNAME, NULL);
	if (!fortytwo_dir){
		pr_info("debugfs_create_dir: %s: failed", FT_DIRNAME);
		return -EAGAIN;
	}

	id_dentry = debugfs_create_file(ID_NAME, ID_MODE, fortytwo_dir, NULL, &id_fops);
	if (!id_dentry) {
		pr_info("debugfs_create_file: %s: failed", ID_NAME);
		return -EAGAIN;
	}

	jiffies_dentry = debugfs_create_file(JIFFIES_NAME, JIFFIES_MODE, fortytwo_dir, NULL, &jiffies_fops);
	if (!jiffies_dentry) {
		pr_info("debugfs_create_file: %s: failed", JIFFIES_NAME);
		return -EAGAIN;
	}
	
	foo_dentry = debugfs_create_file(FOO_NAME, FOO_MODE, fortytwo_dir, NULL, &foo_fops);
	if (!foo_dentry) {
		pr_info("debugfs_create_file: %s: failed", FOO_NAME);
		return -EAGAIN;
	}

	foo_storage = kmalloc(PAGE_SIZE, GFP_USER);
	if (!foo_storage)
		return -ENOMEM;
	return 0;
}

void cleanup_learn_debugfs(void) {
	kfree(foo_storage);
	debugfs_remove_recursive(fortytwo_dir);
	pr_info("Cleanning up learn_debugfs.\n");
}

module_init(init_learn_debugfs);
module_exit(cleanup_learn_debugfs);


