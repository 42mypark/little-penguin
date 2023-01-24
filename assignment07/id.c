#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>

#define LOGIN "mypark"
#define LOGIN_LEN 6

MODULE_LICENSE("GPL");

ssize_t id_read(struct file *file, char __user *buf, size_t, loff_t *offset) {
	int error;

	(void)file;
	(void)offset;
	pr_info("id_read called\n");
	error = copy_to_user(buf, LOGIN, LOGIN_LEN);
	if (error)
		return LOGIN_LEN - error;
	return LOGIN_LEN;
}

ssize_t id_write(struct file *file, const char __user *buf, size_t, loff_t *offset) {
	char kbuf[6] = {0};
	int error;

	pr_info("id_write called\n");
	error = __copy_from_user(kbuf, buf, LOGIN_LEN);
	if (error)
		return -EAGAIN;
	if (strncmp(kbuf, LOGIN, LOGIN_LEN))
		return -EINVAL;
	return LOGIN_LEN;
}

int id_open(struct inode *inode, struct file *file) {
	pr_info("id_open called\n");
	return 0;
}

int id_release(struct inode *inode, struct file *file) {
	pr_info("id_release called\n");
	return 0;
}

const struct file_operations id_fops = {
	.owner		= THIS_MODULE,
	.read		= id_read,
	.write		= id_write,
	.open		= id_open,
	.release	= id_release,
};

struct dentry *id_dentry;

