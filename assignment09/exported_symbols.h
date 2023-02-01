/* SPDX-License-Identifier: GPL-3.0+ */
#ifndef EXPORTED_SYMBOLS_H
#define EXPORTED_SYMBOLS_H
extern struct seq_operations *mnt_op;
extern void (*myput_mnt_ns)(struct mnt_namespace *ns);
extern void (*mymnt_cursor_del)(struct mnt_namespace *ns, struct mount *cursor);
extern int (*myseq_path_root)(struct seq_file *s, struct path *p, struct path *r, const char *e);
#endif
