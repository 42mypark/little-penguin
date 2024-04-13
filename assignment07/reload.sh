#!/bin/bash
NAME=ft_debugfs.ko

rmmod $NAME

dmesg --clear

insmod $NAME

