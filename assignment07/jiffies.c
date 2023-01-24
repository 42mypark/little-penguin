#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/jiffies.h>

#define JIFFIES_LEN_MAX 10

MODULE_LICENSE("GPL");

void jiffies_to_buf(unsigned long js, char *buf) {
	for (int d = JIFFIES_LEN_MAX - 1; d >= 0; --d) {
		buf[d] = '0' + js % 10;
		js /= 10;
	}
}

ssize_t jiffies_read(struct file *file, char __user *buf, size_t len, loff_t *offset) {
	int error;
	unsigned long js = jiffies;
	char js_buf[JIFFIES_LEN_MAX] = {0};

	(void)file;
	(void)offset;
	pr_info("jiffies_read called\n");
	pr_info("\tjiffies = %lu, len = %zu", js, len);

	if (len < JIFFIES_LEN_MAX)
		return -EINVAL;

	jiffies_to_buf(js, js_buf);
	error = copy_to_user(buf, js_buf, JIFFIES_LEN_MAX); // 입력으로 받은 buf가 제대로 된 주소인지?
	if (error)
		return JIFFIES_LEN_MAX - error;
	return JIFFIES_LEN_MAX;
}

ssize_t jiffies_write(struct file *file, const char __user *buf, size_t len, loff_t *offset) {
	pr_info("jiffies_write called\n");
	return 0;
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
	.write		= jiffies_write,
	.open		= jiffies_open,
	.release	= jiffies_release,
};

struct dentry *jiffies_dentry;

