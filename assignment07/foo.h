#ifndef FOO_H
#define FOO_H

#define FOO_NAME "foo"
#define FOO_MODE 0644
extern const struct file_operations foo_fops;
extern struct dentry *foo_dentry;
extern void *foo_storage;

#endif
   
