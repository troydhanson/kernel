#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/list.h>

/* module information */
MODULE_AUTHOR("Troy D. Hanson");
MODULE_DESCRIPTION("Example of linked list");
MODULE_LICENSE("Dual BSD/GPL");

static int items=0;
#ifdef MODULE
module_param(items, int, 0644);
MODULE_PARM_DESC(items, "number of items in linked list");
#endif

struct kex_list_item {
  struct list_head list;
  int ordinal;
};

/* global state. must be thread safe. */
struct list_head kex_list;

void free_kex_list(void) {
  struct list_head *e, *n;
  struct kex_list_item *k;

  printk(KERN_INFO "releasing kex list\n");

  list_for_each_safe(e, n, &kex_list) {
    k = (struct kex_list_item*)list_entry(e, struct kex_list_item, list);
    printk(KERN_INFO "free kex list item %d\n", k->ordinal);
    list_del(e);
    kfree(k);
  }
  printk(KERN_INFO "list_empty:%d\n", list_empty(&kex_list));
}

int __init kex_init(void) {
  struct list_head *e;
  struct kex_list_item *k;
  int i, rc;

  printk(KERN_INFO "module_init: building linked list of %d items\n", items);

  INIT_LIST_HEAD(&kex_list);

  /* add items to our list */
  for(i=0; i < items; i++) {
    k = (struct kex_list_item*)kmalloc(sizeof(*k), GFP_KERNEL);
    if (k==NULL) goto done;
    k->ordinal = i;
    list_add(&k->list, &kex_list);
  }

  /* iterate over the list */
  printk(KERN_INFO "iterating over list\n");
  list_for_each(e, &kex_list) {
    k = (struct kex_list_item*)list_entry(e, struct kex_list_item, list);
    printk(KERN_INFO "item %d\n", k->ordinal);
  }

  rc = 0;

 done:
  if (rc < 0) free_kex_list();
  return rc ? -ENOMEM : 0;
}

void __exit kex_cleanup(void) {
  printk(KERN_INFO "module exit\n");
  free_kex_list();
}

module_init(kex_init);
module_exit(kex_cleanup);
