#include <linux/mount.h>
#include <../fs/mount.h>
#include <linux/seq_file.h>


int mymnt_seq_show(struct seq_file *seq, void *v)
{
	struct proc_mounts *pm;
	struct vfsmount *vmnt = NULL; // ?
	
	pm = seq->private;
	return pm->show(seq, vmnt);
}

void mymnt_seq_stop(struct seq_file *seq, void *v)
{
}

void *mymnt_seq_start(struct seq_file *seq, loff_t *pos)
{
	return NULL;
}

void *mymnt_seq_next(struct seq_file *seq, void *v, loff_t *pos)
{
	return NULL;
}



const struct seq_operations mymnt_seq_ops = {
	.start = mymnt_seq_start,
	.stop = mymnt_seq_stop,
	.next = mymnt_seq_next,
	.show = mymnt_seq_show,
};
