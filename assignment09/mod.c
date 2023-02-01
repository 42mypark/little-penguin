// SPDX-License-Identifier: GPL-3.0+
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/spinlock_types.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/kprobes.h>
#include <linux/mnt_namespace.h>
#include <../fs/mount.h>
#include "mymnt.h"

#define GET_SYMBOL(NAME, VAL)  {	\
	unsigned long long  err;	\
	err = find_symbols(NAME);	\
	if (!err) 			\
		return -EINVAL;		\
	VAL = (typeof(VAL))err;		\
}					\


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("");

typedef int (*fop_func_t)(struct inode*, struct file*);

struct proc_dir_entry *entry;
struct seq_operations *mnt_op;
void (*myput_mnt_ns)(struct mnt_namespace *ns);
void (*mymnt_cursor_del)(struct mnt_namespace *ns, struct mount *cursor);
int  (*myseq_path_root)(struct seq_file*, struct path*, struct path*, const char*);


unsigned long (*find_symbols)(const char *);

int mymnt_init(void)
{
	struct kprobe kp;
	unsigned long long err;

	memset(&kp, 0, sizeof(kp));
	kp.symbol_name = "kallsyms_lookup_name";
	err = register_kprobe(&kp);
	if (err)
		return -EINVAL;
	find_symbols = (typeof(find_symbols))kp.addr;
	unregister_kprobe(&kp);

	GET_SYMBOL("mounts_op", mnt_op);
	GET_SYMBOL("put_mnt_ns", myput_mnt_ns);
	GET_SYMBOL("mnt_cursor_del", mymnt_cursor_del);
//	GET_SYMBOL("show_type", myshow_type);
//	GET_SYMBOL("mangle", mymangle);
	GET_SYMBOL("seq_path_root", myseq_path_root);

	entry = proc_create("mymounts", 0444, NULL, &mymnt_proc_ops);
	return 0;
}

void mymnt_cleanup(void)
{
	proc_remove(entry);
}

module_init(mymnt_init);
module_exit(mymnt_cleanup);
