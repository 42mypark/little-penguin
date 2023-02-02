// SPDX-License-Identifier: GPL-3.0+
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/debugfs.h>
#include <linux/spinlock_types.h>
#include "parent_dir.h"

#define FOO_NAME "foo"
#define FOO_MODE 0644

MODULE_LICENSE("GPL");


static size_t num;
static long long w_pos;
static struct dentry *foo_dentry;
static void *foo_storage;
DEFINE_SPINLOCK(lock);

ssize_t foo_read(struct file *file, char __user *buf, size_t len, loff_t *ppos)
{
	int error;

	spin_lock(&lock);

	pr_info("read: num-%zu\n", *(size_t *)file->private_data);
	len = *ppos + len  <= w_pos ? len : w_pos - *ppos;
	error = copy_to_user(buf, foo_storage + *ppos, len);
	len -= error;
	*ppos += len;
	pr_info("read: num-%zu\n", *(size_t *)file->private_data);
	spin_unlock(&lock);

	pr_info("read: pos: %lld\n", *ppos);

	return len;
}

ssize_t foo_write(struct file *file, const char __user *buf, size_t len, loff_t *ppos)
{
	int error;

	pr_info("write:\n");
	pr_info("\tb:w_pos: %lld\n", w_pos);

	spin_lock(&lock);
	if (w_pos == PAGE_SIZE) {
		spin_unlock(&lock);
		return -ENOSPC;
	}
	len = w_pos + len <= PAGE_SIZE ? len : PAGE_SIZE - w_pos;
	error = __copy_from_user(foo_storage + w_pos, buf, len);
	len -= error;
	w_pos += len;
	spin_unlock(&lock);

	pr_info("\ta:w_pos: %lld\n", w_pos);

	return len;
}

int foo_open(struct inode *inode, struct file *file)
{
	file->private_data = kmalloc(sizeof(size_t), GFP_USER);
	spin_lock(&lock);
	pr_info("open: w_pos: %lld\n", w_pos);
	*(size_t *)file->private_data = num;
	num++;
	spin_unlock(&lock);
	return 0;
}

int foo_release(struct inode *inode, struct file *file)
{
	spin_lock(&lock);
	pr_info("release: w_pos: %lld, pos: %lld\n", w_pos, file->f_pos);
	spin_unlock(&lock);
	return 0;
}

static const struct file_operations foo_fops = {
	.owner		= THIS_MODULE,
	.read		= foo_read,
	.write		= foo_write,
	.open		= foo_open,
	.release	= foo_release,
};

int foo_init(void)
{
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

void foo_exit(void)
{
	kfree(foo_storage);
}


