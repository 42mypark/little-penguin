# SPDX-License-Identifier: GPL-3.0+
NAME=ft_debugfs.ko

rmmod $NAME
insmod $NAME

dmesg --clear
