#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/spinlock_types.h>

MODULE_LICENSE("GPL");

// lock ? 

static long long int w_pos;
struct dentry *foo_dentry;
void *foo_storage;
DEFINE_SPINLOCK(lock);

ssize_t foo_read(struct file *file, char __user *buf, size_t len, loff_t *ppos) {
	int error, move;

	pr_info("foo_read called\n");

	move = *ppos + len  <= w_pos ? len : w_pos - *ppos;

	spin_lock(&lock);
	error = copy_to_user(buf, foo_storage + *ppos, move);
	spin_unlock(&lock);

	move = len - error;
	*ppos += move;

	pr_info("\tpos: %lld\n", *ppos);

	return move;
}

ssize_t foo_write(struct file *file, const char __user *buf, size_t len, loff_t *ppos) {
	int error, move;

	pr_info("foo_write called\n");

	if (w_pos == PAGE_SIZE)
		return -ENOSPC;

	move = w_pos + len <= PAGE_SIZE ? len : PAGE_SIZE - w_pos;

	spin_lock(&lock);
	error = __copy_from_user(foo_storage + w_pos, buf, move);
	spin_unlock(&lock);

	move = len - error;
	w_pos += move;

	pr_info("\tw_pos: %lld\n", w_pos);

	return move;
}

int foo_open(struct inode *inode, struct file *file) {
	pr_info("foo_open called\n");
	return 0;
}

int foo_release(struct inode *inode, struct file *file) {
	pr_info("foo_release called\n");
	return 0;
}

const struct file_operations foo_fops = {
	.owner		= THIS_MODULE,
	.read		= foo_read,
	.write		= foo_write,
	.open		= foo_open,
	.release	= foo_release,
};


