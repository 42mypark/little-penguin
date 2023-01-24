#ifndef JIFFIES_H
#define JIFFIES_H

#define JIFFIES_NAME "jiffies"
#define JIFFIES_MODE 0444
extern const struct file_operations jiffies_fops;
extern struct dentry *jiffies_dentry;

#endif
   
