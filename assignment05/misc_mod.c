// SPDX-License-Identifier: GPL-3.0+
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>

#define LOGIN "mypark"
#define LOGIN_LEN 6

MODULE_LICENSE("GPL");

ssize_t mymisc_read(struct file *, char __user *buf, size_t len, loff_t *ppos)
{
	return simple_read_from_buffer(buf, len, ppos,  LOGIN, LOGIN_LEN);
}

ssize_t mymisc_write(struct file *, const char __user *buf, size_t len, loff_t *ppos)
{
	char kbuf[LOGIN_LEN] = {0};
	int error;

	len = len <= LOGIN_LEN ? len : LOGIN_LEN;
	error = __copy_from_user(kbuf, buf, len);
	if (error)
		return -EAGAIN;
	if (strncmp(kbuf, LOGIN, LOGIN_LEN))
		return -EINVAL;
	return LOGIN_LEN;
}

static const struct file_operations mymisc_fops = {
	.owner		= THIS_MODULE,
	.read		= mymisc_read,
	.write		= mymisc_write,
};

static struct miscdevice misc_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "fortytwo",
	.fops = &mymisc_fops,
	.nodename = "fortytwo",
	.mode = 0644,
};

int __init init_mymisc(void)
{
	int err;

	pr_info("Hello misc !\n");
	err = misc_register(&misc_dev);
	if (err) {
		pr_info("misc_register failed\n");
		return err;
	}
	pr_info("misc_register success\n");
	return 0;
}

void __exit cleanup_mymisc(void)
{
	misc_deregister(&misc_dev);
	pr_info("Cleanning up misc.\n");
}

module_init(init_mymisc);
module_exit(cleanup_mymisc);

