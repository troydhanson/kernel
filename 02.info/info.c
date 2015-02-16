#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

/* module information */
MODULE_AUTHOR("Troy D. Hanson");
MODULE_DESCRIPTION("Example of module information");
MODULE_LICENSE("Dual BSD/GPL");


/* __init and __exit make it work as a static driver too */
int __init info_init(void) {
  printk("info module init called\n");
  return 0;
}

void __exit info_cleanup(void) {
  printk("info module exit called\n");
}

/* module_init and module_exit make it work as a loadable module */
module_init(info_init);
module_exit(info_cleanup);
