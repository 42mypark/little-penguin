#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/fs.h>

#define LOGIN "mypark"
#define LOGIN_LEN 6
#define ID_NAME "id"
#define ID_MODE 0665

MODULE_LICENSE("GPL");

extern struct dentry *fortytwo_dir;
static struct dentry *id_dentry;

ssize_t id_read(struct file *, char __user *buf, size_t len, loff_t *ppos) {
	pr_info("id_read called\n");
	return simple_read_from_buffer(buf, len, ppos, LOGIN, LOGIN_LEN);
}

ssize_t id_write(struct file *file, const char __user *buf, size_t len, loff_t *) {
	char kbuf[LOGIN_LEN] = {0};
	int error;

	pr_info("id_write called\n");

	len = len <= LOGIN_LEN ? len : LOGIN_LEN;
	error = __copy_from_user(kbuf, buf, len);
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

static const struct file_operations id_fops = {
	.owner		= THIS_MODULE,
	.read		= id_read,
	.write		= id_write,
	.open		= id_open,
	.release	= id_release,
};


int id_init(void) {
	id_dentry = debugfs_create_file(ID_NAME, ID_MODE, fortytwo_dir, NULL, &id_fops);
	if (!id_dentry) {
		pr_info("debugfs_create_file: %s: failed", ID_NAME);
		return -EAGAIN;
	}
	return 0;
}

