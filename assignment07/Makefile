TEST_DIR	:= tests
TEST_SRC	:= test_main.c test_utils.c test_id.c test_jiffies.c  test_foo.c
TEST_SRC_POS	:= $(addprefix $(TEST_DIR)/, $(TEST_SRC))
TEST_NAME	:= test
RM		:= rm -rfv

all:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	$(RM) $(TEST_NAME)
install:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules_install

$(TEST_NAME): $(TEST_SRC_POS)
	$(CC) $^ -o $@ -I $(TEST_DIR)

.PHONY: all clean install

	

