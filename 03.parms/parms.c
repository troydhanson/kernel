#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

/* module information */
MODULE_AUTHOR("Troy D. Hanson");
MODULE_DESCRIPTION("Example of module parameters");
MODULE_LICENSE("Dual BSD/GPL");


static int verbose=0;
#ifdef MODULE
module_param(verbose, int, 0644);
MODULE_PARM_DESC(verbose, "set verbosity level");
#endif

/* __init and __exit make it work as a static driver too */
int __init parms_init(void) {
  printk("parms module init called (verbosity %u)\n",verbose);
  return 0;
}

void __exit parms_cleanup(void) {
  printk("parms module exit called (verbosity %u)\n",verbose);
}

/* module_init and module_exit make it work as a loadable module */
module_init(parms_init);
module_exit(parms_cleanup);
