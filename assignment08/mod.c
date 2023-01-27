// SPDX-License-Identifier: GPL-3.0+
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Louis Solofrizzo <louis@ne02ptzero.me>");
MODULE_DESCRIPTION("Useless module");

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

static char str[PAGE_SIZE];
static char *tmp;

int __init myfd_init(void)
{
	tmp = kmalloc(sizeof(char) * PAGE_SIZE, GFP_KERNEL);
	return misc_register(&myfd_device);
}

void __exit myfd_cleanup(void)
{
	kfree(tmp);
}

/*
 * Malloc like a boss
 */
ssize_t myfd_read(struct file *fp, char __user *user, size_t size, loff_t *offs)
{
	size_t t, i;
	ssize_t ret;

	len = strlen(str);
	for (i = 0; i < len; i++)
		tmp[i] = str[len - 1 - i];
	tmp[len] = 0;
	ret = simple_read_from_buffer(user, size, offs, tmp, i);
	return ret;
}

ssize_t myfd_write(struct file *fp, const char __user *user, size_t size, loff_t *offs)
{
	ssize_t res;

	if (size >= PAGE_SIZE)
		return -EINVAL;
	res = simple_write_to_buffer(str, size, offs, user, size);
	str[res] = 0;
	return res;
}

module_init(myfd_init);
module_exit(myfd_cleanup);
