#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/jiffies.h>
#include <linux/debugfs.h>

#define JIFFIES_LEN_MAX 10
#define JIFFIES_NAME "jiffies"
#define JIFFIES_MODE 0444

MODULE_LICENSE("GPL");

extern struct dentry *fortytwo_dir;
static struct dentry *jiffies_dentry;

static void jiffies_to_buf(unsigned long js, char *buf) {
	for (int d = JIFFIES_LEN_MAX - 1; d >= 0; --d) {
		buf[d] = '0' + js % 10;
		js /= 10;
	}
}

ssize_t jiffies_read(struct file *, char __user *buf, size_t len, loff_t *) {
	int error;
	unsigned long js = jiffies;
	char js_buf[JIFFIES_LEN_MAX] = {0};

	pr_info("jiffies_read called\n");

	if (len < JIFFIES_LEN_MAX)
		return -EINVAL;

	jiffies_to_buf(js, js_buf);
	error = copy_to_user(buf, js_buf, JIFFIES_LEN_MAX);
	return JIFFIES_LEN_MAX - error;
}

int jiffies_open(struct inode *inode, struct file *file) {
	pr_info("jiffies_open called\n");
	return 0;
}

int jiffies_release(struct inode *inode, struct file *file) {
	pr_info("jiffies_release called\n");
	return 0;
}

const struct file_operations jiffies_fops = {
	.owner		= THIS_MODULE,
	.read		= jiffies_read,
	.open		= jiffies_open,
	.release	= jiffies_release,
};


int jiffies_init(void) {
	jiffies_dentry = debugfs_create_file(JIFFIES_NAME, JIFFIES_MODE, fortytwo_dir, NULL, &jiffies_fops);
	if (!jiffies_dentry) {
		pr_info("debugfs_create_file: %s: failed", JIFFIES_NAME);
		return -EAGAIN;
	}
	return 0;
}



