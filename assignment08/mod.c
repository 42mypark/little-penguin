// SPDX-License-Identifier: GPL-3.0+
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/spinlock_types.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Louis Solofrizzo <louis@ne02ptzero.me>");
MODULE_DESCRIPTION("revserse string.");

static char str[PAGE_SIZE];
static loff_t w_pos;
DEFINE_SPINLOCK(str_lock);

static ssize_t myfd_read(struct file *fp, char __user *user, size_t size, loff_t *offs);
static ssize_t myfd_write(struct file *fp, const char __user *user, size_t size, loff_t *offs);

const struct file_operations myfd_fops = {
	.owner = THIS_MODULE,
	.read = &myfd_read,
	.write = &myfd_write
};

static struct miscdevice myfd_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "reverse",
	.fops = &myfd_fops
};


int __init myfd_init(void)
{
	return misc_register(&myfd_device);
}

void __exit myfd_cleanup(void)
{
}

/*
 * Malloc like a boss
 */
ssize_t myfd_read(struct file *fp, char __user *user, size_t size, loff_t *offs)
{
	size_t i, s;
	ssize_t ret, len;
	char *tmp;

	spin_lock(&str_lock);
	len = w_pos - *offs;
	s = sizeof(char) * len;
	tmp = kmalloc(s, GFP_KERNEL);
	if (tmp) {
		spin_unlock(&str_lock);
		return -ENOMEM;
	}
	for (i = 0; i < len; i++)
		tmp[i] = str[len - 1 - i];
	spin_unlock(&str_lock);
	tmp[len] = 0;
	ret = simple_read_from_buffer(user, size, offs, tmp, len);
	kfree(tmp);
	return ret;
}

ssize_t myfd_write(struct file *fp, const char __user *user, size_t size, loff_t *offs)
{
	ssize_t res;

	spin_lock(&str_lock);
	if (size + w_pos >= PAGE_SIZE) {
		spin_unlock(&str_lock);
		return -EINVAL;
	}
	res = simple_write_to_buffer(str + w_pos, size, &w_pos, user, size);
	str[w_pos] = 0;
	spin_unlock(&str_lock);
	return res;
}

module_init(myfd_init);
module_exit(myfd_cleanup);
