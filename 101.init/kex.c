#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

/* __init and __exit make it work as a static driver too */
int __init kex_init(void) {
  printk("kex module init called\n");
  return 0;
}

void __exit kex_cleanup(void) {
  printk("kex module exit called\n");
}

/* module_init and module_exit make it work as a loadable module */
module_init(kex_init);
module_exit(kex_cleanup);
