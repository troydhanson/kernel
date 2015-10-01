#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

/* module information */
MODULE_AUTHOR("Troy D. Hanson");
MODULE_DESCRIPTION("Example of module information");
MODULE_LICENSE("Dual BSD/GPL");


/* __init and __exit make it work as a static driver too */
int __init kex_init(void) {
  printk("kex module init called\n");
  printk("HZ is %u\n", HZ);
  printk("jiffies is %lu\n", jiffies);
  printk("jiffies_64 is %lu\n", (unsigned long)jiffies_64);
  return 0;
}

void __exit kex_cleanup(void) {
  printk("kex module exit called\n");
}

/* module_init and module_exit make it work as a loadable module */
module_init(kex_init);
module_exit(kex_cleanup);
