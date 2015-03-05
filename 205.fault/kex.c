#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/mm.h>

/* module information */
MODULE_AUTHOR("Troy D. Hanson");
MODULE_DESCRIPTION("Example of character device");
MODULE_LICENSE("Dual BSD/GPL");

#define NUM_MINORS 1

DEFINE_MUTEX(mutex);

struct chardev_t {
  dev_t dev;        /* has major and minor bits */
  struct cdev cdev; /* has our ops, owner, etc */
} c;

static 
struct page *fault(struct vm_area_struct *vma, unsigned long addr, int *type) {
  struct page *page=NULL;
  //get_page();
  if (type) *type=VM_FAULT_MINOR;
  return page;
}

/* ops for VMA open, close and fault. See Linux Device Drivers, 3rd ed, p427. */
static struct vm_operations_struct vm_ops = {
  .nopage = fault, /* page fault handler */
};

int _mmap(struct file *f, struct vm_area_struct *vma) {
  unsigned long pages = (vma->vm_end - vma->vm_start) >> PAGE_SHIFT;
  int rc=-ENODEV;
  printk(KERN_DEBUG "vma->vm_end %lu vm_start %lu len %lu pages %lu vm_pgoff %lu\n",
    vma->vm_end, vma->vm_start, vma->vm_end - vma->vm_start, pages, vma->vm_pgoff);

  if (0) goto done;
  vma->vm_ops = &vm_ops;
  vma->vm_private_data = (void*)0xa1fa1fa; /* could store private data here */
  
  rc = 0;

 done:
  return rc;
}

struct file_operations ops = {
  .mmap = _mmap
};

int __init chardev_init(void) {
  int rc;

  /* ask for a dynamic major */
  rc = alloc_chrdev_region(&c.dev, 0, NUM_MINORS, "kex");
  if (rc) { 
    rc = -ENODEV;
    goto done; 
  }

  /* init the struct cdev */
  cdev_init(&c.cdev, &ops);
  c.cdev.owner = THIS_MODULE;

  /* make device live */
  rc = cdev_add(&c.cdev, c.dev, NUM_MINORS);
  if (rc) {
    rc = -ENODEV;
    printk(KERN_WARNING "cdev_add: can't add device\n");
    unregister_chrdev_region(c.dev, NUM_MINORS);
    cdev_del(&c.cdev);
    goto done;
  }

  rc = 0;

 done:
  return rc;
}

void __exit chardev_cleanup(void) {
  cdev_del(&c.cdev);
  unregister_chrdev_region(c.dev, NUM_MINORS);
}

module_init(chardev_init);
module_exit(chardev_cleanup);
