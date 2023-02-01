#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>

#define LOGIN "mypark"
#define LOGIN_LEN 6

MODULE_LICENSE("GPL");

ssize_t mymisc_read(struct file *file, char __user *buf, size_t, loff_t *offset) {
	int error;

	(void)file;
	(void)offset;
	pr_info("mymisc_read called\n");
	error = copy_to_user(buf, LOGIN, LOGIN_LEN);
	return LOGIN_LEN - error;
}

ssize_t mymisc_write(struct file *file, const char __user *buf, size_t, loff_t *offset) {
	char kbuf[6] = {0};
	int error;

	pr_info("mymisc_write called\n");
	error = __copy_from_user(kbuf, buf, LOGIN_LEN);
	if (error)
		return -EAGAIN;
	if (strncmp(kbuf, LOGIN, LOGIN_LEN))
		return -EINVAL;
	return LOGIN_LEN;
}

int mymisc_open(struct inode *inode, struct file *file) {
	pr_info("mymisc_open called\n");
	return 0;
}

int mymisc_release(struct inode *inode, struct file *file) {
	pr_info("mymisc_release called\n");
	return 0;
}

static const struct file_operations mymisc_fops = {
	.owner		= THIS_MODULE,
	.read		= mymisc_read,
	.write		= mymisc_write,
	.open		= mymisc_open,
	.release	= mymisc_release,
};

struct miscdevice misc_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "fortytwo",
	.fops = &mymisc_fops,
	.nodename = "fortytwo",
	.mode = 0644,
};

int __init init_mymisc(void) {
	int err;

	pr_info("Hello misc ! \n");
	err = misc_register(&misc_dev);
	if (err) {
		pr_info("misc_register failed\n");
		return err;
	}
	pr_info("misc_register success\n");
	return 0;
}

void __exit cleanup_mymisc(void) {
	misc_deregister(&misc_dev);
	pr_info("Cleanning up misc.\n");
}

module_init(init_mymisc);
module_exit(cleanup_mymisc);

