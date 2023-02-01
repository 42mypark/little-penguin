NAME=mymounts.ko

dmesg --clear

rmmod $NAME
insmod $NAME

dmesg

