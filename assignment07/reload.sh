NAME=ft_debugfs.ko

rmmod $NAME
insmod $NAME

dmesg --clear
