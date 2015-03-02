#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

/* __init and __exit make it work as a static driver too */
int __init basic_init(void) {
  printk("basic module init called\n");
  return 0;
}

void __exit basic_cleanup(void) {
  printk("basic module exit called\n");
}

/* module_init and module_exit make it work as a loadable module */
module_init(basic_init);
module_exit(basic_cleanup);
