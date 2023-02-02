// SPDX-License-Identifier: GPL-3.0+
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/fs.h>
#include "parent_dir.h"

#define LOGIN "mypark"
#define LOGIN_LEN 6
#define ID_NAME "id"
#define ID_MODE 0665

MODULE_LICENSE("GPL");

static struct dentry *id_dentry;

ssize_t id_read(struct file *, char __user *buf, size_t len, loff_t *ppos)
{
	return simple_read_from_buffer(buf, len, ppos, LOGIN, LOGIN_LEN);
}

ssize_t id_write(struct file *file, const char __user *buf, size_t len, loff_t *)
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

static const struct file_operations id_fops = {
	.owner		= THIS_MODULE,
	.read		= id_read,
	.write		= id_write,
};


int id_init(void)
{
	id_dentry = debugfs_create_file(ID_NAME, ID_MODE, fortytwo_dir, NULL, &id_fops);
	if (!id_dentry) {
		pr_info("debugfs_create_file: %s: failed", ID_NAME);
		return -EAGAIN;
	}
	return 0;
}

