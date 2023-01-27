RM		:= rm -rfv

all:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	$(RM) $(TEST)
install:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules_install

$(TEST_NAME): $(TEST_SRC_POS)
	$(CC) $^ -o $@ -I $(TEST_DIR)

.PHONY: all clean install

	

