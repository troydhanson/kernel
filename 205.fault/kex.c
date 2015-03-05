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

struct chardev_t {
  dev_t dev;        /* has major and minor bits */
  struct cdev cdev; /* has our ops, owner, etc */
} c;

int kex_fault(struct vm_area_struct *vma, struct vm_fault *vmf) {
  int flags = 0; /* VM_FAULT_MINOR. see mm.h */

  printk(KERN_DEBUG "fault on %p (userspace:%d write:%d)\n", 
    (void*)vmf->virtual_address,
    (vmf->flags & FAULT_FLAG_USER)  ? 1:0,
    (vmf->flags & FAULT_FLAG_WRITE) ? 1:0);

  vmf->page = alloc_page(GFP_KERNEL);
  if (!vmf->page) { 
    flags = VM_FAULT_OOM; 
    goto done; 
  }

  // XXX
  //get_page() not needed; -- alloc_page already took a reference
  //@unmap does __free_page - that should drop refcount to 0

 done:
  return flags; // VM_FAULT_MINOR
}

/* see linux/include/linux/mm.h - this struct has changed since LDD3 p.427 */
static struct vm_operations_struct vm_ops = {
  // .open = kex_open,
  // .close = kex_close,
  .fault = kex_fault,
};

int _mmap(struct file *f, struct vm_area_struct *vma) {
  unsigned long pages = (vma->vm_end - vma->vm_start) >> PAGE_SHIFT;
  int rc=-ENODEV;
  printk(KERN_DEBUG "vma->vm_end %lu vm_start %lu len %lu pages %lu vm_pgoff %lu\n",
    vma->vm_end, vma->vm_start, vma->vm_end - vma->vm_start, pages, vma->vm_pgoff);

  vma->vm_ops = &vm_ops;
  vma->vm_private_data = (void*)0xa1fa1fa; /* can store private data here */
  
  rc = 0;

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
