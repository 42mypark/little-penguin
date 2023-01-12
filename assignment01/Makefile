obj-m = hello.o
all:
	$(Make) -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
clean:
	$(Make) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
