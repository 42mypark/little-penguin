NAME=learn_debugfs.ko

rmmod $NAME
insmod $NAME

dmesg --clear
