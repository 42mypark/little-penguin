// SPDX-License-Identifier: GPL-3.0+
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/mount.h>
#include <../fs/mount.h>


MODULE_LICENSE("GPL");

struct mountinfo {
	struct list_head list;
	const char *name;
	const char *d_name;
};


int init_mod(void) {
       	struct file *f = filp_open("/", O_RDONLY, 0644);
	struct dentry *d = file_dentry(f);
	struct super_block *sb = d->d_sb;
	struct list_head *curr = &sb->s_list;
	struct list_head *head = curr;
	struct list_head *m_curr;
	struct list_head *m_head;
	struct mount *m;

	int cnt = 0;
	curr = curr->next;
	while (curr != head) {
		cnt++;
		sb = container_of(curr , struct super_block, s_list);
		if (sb == NULL ||  sb->s_type == NULL) {
			curr = curr->next;
			continue;
		}
		pr_info("type: %s\n", sb->s_type->name);

		m_head = &sb->s_mounts;
		m_curr = m_head->next;
		while (m_curr != m_head) {
			m = container_of(m_curr, struct mount, mnt_instance);
			if (m == NULL || m->mnt_mp == NULL) {
				m_curr = m_curr->next;
				continue;
			}
			pr_info("dev: %s, pos: %s\n", m->mnt_devname,  m->mnt_mp->m_dentry->d_name.name);
			m_curr = m_curr->next;
		}
		m = container_of(m_curr, struct mount, mnt_instance);
		if (m == NULL || m->mnt_mp == NULL) {
			curr = curr->next;
			continue;
		}
		curr = curr->next;
	}

	sb = container_of(curr , struct super_block, s_list);
	pr_info("type: %s\n", sb->s_type->name);
	pr_info("%d\n",cnt);

	return 0;
}

void exit_mod(void) {
}

module_init(init_mod);
module_exit(exit_mod);
