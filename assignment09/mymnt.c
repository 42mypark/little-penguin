// SPDX-License-Identifier: GPL-3.0+
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/fs_struct.h>
#include <linux/sched.h>
#include <linux/nsproxy.h>
#include <linux/mnt_namespace.h>

#include <../fs/internal.h>
#include <../fs/mount.h>

#include "exported_symbols.h"

static int mymnt_proc_show(struct seq_file *seq, struct vfsmount *vmnt)
{
	struct proc_mounts *pm = seq->private;
	struct mount *mnt = real_mount(vmnt);
	struct super_block *sb = vmnt->mnt_sb;
	struct path mnt_path = { .dentry = vmnt->mnt_root, .mnt = vmnt };
	int err = 0;


	seq_puts(seq, sb->s_type->name);
	seq_puts(seq, ": ");
	seq_puts(seq, mnt->mnt_devname ? mnt->mnt_devname : "none");
	seq_puts(seq, ": ");
	err = myseq_path_root(seq, &mnt_path, &pm->root, " \t\n\\");
	if (err)
		return err;
	seq_putc(seq, '\n');
	return 0;
}

static int get_properties(struct inode *inode, struct mnt_namespace **ns, struct path *root)
{
	int ret = -EINVAL;

	struct task_struct *task = current;
	struct nsproxy *nsp;

	if (!task)
		goto ret;

	task_lock(task);
	nsp = task->nsproxy;
	if (!nsp || !nsp->mnt_ns)
		goto cleanup_task;
	if (!task->fs)
		goto err_fs;
	*ns = nsp->mnt_ns;
	get_mnt_ns(*ns);             // cleanup when release
	get_fs_root(task->fs, root); // cleanup when release
	ret = 0;
	goto cleanup_task;
err_fs:
	ret = -ENOENT;
cleanup_task:
	task_unlock(task);
ret:
	return ret;
}


static int mymnt_proc_open(struct inode *inode, struct file *file)
{
	int err;
	struct proc_mounts *pm;
	struct seq_file *sf;

	struct mnt_namespace *ns;
	struct path root;

	err = get_properties(inode, &ns, &root);
	if (err)
		return err;
	err = seq_open_private(file, mnt_op, sizeof(struct proc_mounts));
	if (err)
		return err;

	sf = file->private_data;
	sf->poll_event = ns->event; // ?

	pm = sf->private;
	pm->ns = ns;
	pm->root = root;
	pm->show = mymnt_proc_show;
	INIT_LIST_HEAD(&pm->cursor.mnt_list);
	pm->cursor.mnt.mnt_flags = MNT_CURSOR;

	return 0;
}

static int mymnt_proc_release(struct inode *inode, struct file *file)
{
	struct seq_file *sf = file->private_data;
	struct proc_mounts *pm = sf->private;

	path_put(&pm->root);
	mymnt_cursor_del(pm->ns, &pm->cursor);
	myput_mnt_ns(pm->ns);
	return seq_release_private(inode, file);
}

// simliar with fops
const struct proc_ops mymnt_proc_ops = {
	.proc_open = mymnt_proc_open,
	.proc_read = seq_read,
	.proc_read_iter = seq_read_iter,
	.proc_lseek = seq_lseek,
	.proc_release = mymnt_proc_release,
};

