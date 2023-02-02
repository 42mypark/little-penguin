# SPDX-License-Identifier: GPL-3.0+
NAME=mymounts.ko

dmesg --clear

rmmod $NAME
sleep 1
insmod $NAME

dmesg
