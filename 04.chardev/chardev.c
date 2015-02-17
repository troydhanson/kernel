#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

/* module information */
MODULE_AUTHOR("Troy D. Hanson");
MODULE_DESCRIPTION("Example of character device");
MODULE_LICENSE("Dual BSD/GPL");


int __init chardev_init(void) {
  return 0;
}

void __exit chardev_cleanup(void) {
}

module_init(chardev_init);
module_exit(chardev_cleanup);
