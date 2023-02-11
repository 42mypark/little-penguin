// SPDX-License-Identifier: GPL-3.0+
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/jiffies.h>
#include <linux/debugfs.h>
#include "parent_dir.h"

#define LEN_MAX 10
#define NAME "jiffies"
#define MODE 0444

MODULE_LICENSE("GPL");

static struct dentry *jiffies_d;

static void jiffies_to_buf(unsigned long js, char *buf)
{
	for (int d = LEN_MAX - 1; d >= 0; --d) {
		buf[d] = '0' + js % 10;
		js /= 10;
	}
}

ssize_t jiffies_read(struct file *, char __user *buf, size_t len, loff_t *)
{
	int error;
	unsigned long js = jiffies;
	char js_buf[LEN_MAX] = {0};

	if (len < LEN_MAX)
		return -EINVAL;

	jiffies_to_buf(js, js_buf);
	error = copy_to_user(buf, js_buf, LEN_MAX);
	return LEN_MAX - error;
}

const struct file_operations jiffies_fops = {
	.owner		= THIS_MODULE,
	.read		= jiffies_read,
};

int jiffies_init(void)
{
	jiffies_d = debugfs_create_file(NAME, MODE, fortytwo_dir, NULL, &jiffies_fops);
	if (!jiffies_d) {
		pr_info("debugfs_create_file: %s: failed", NAME);
		return -EAGAIN;
	}
	return 0;
}
