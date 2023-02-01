TEST_SRC	:= test.c
TEST		:= test
RM		:= rm -rfv

all:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	$(RM) $(TEST)
install:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules_install

$(TEST): $(TEST_SRC)
	$(CC) $(TEST_SRC) -o $(TEST)

.PHONY: all clean install

