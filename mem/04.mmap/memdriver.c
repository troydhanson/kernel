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

/* a global to keep state. must be thread safe. */
struct chardev_t {
  dev_t dev;        /* has major and minor bits */
  struct cdev cdev; /* has our ops, owner, etc */
} c;

int _open(struct inode *inode, struct file *f) {
  return 0;
}

int _release(struct inode *inode, struct file *f) {
  return 0;
}

ssize_t _read(struct file *f, char __user *buf, size_t count, loff_t *offp) {
  return 0;
}

ssize_t _write(struct file *f, const char __user *buf, size_t count, 
               loff_t *offp) {
  return -EINVAL;
}

int _mmap(struct file *f, struct vm_area_struct *vma) {
  unsigned long pages = (vma->vm_end - vma->vm_start) >> PAGE_SHIFT;
  unsigned long addr;
  struct page *page;
  int i,rc=-ENODEV;
  // TODO get semaphore 
  printk(KERN_DEBUG "vma->vm_end %lu vm_start %lu len %lu pages %lu vm_pgoff %lu\n",
    vma->vm_end, vma->vm_start, vma->vm_end - vma->vm_start, pages, vma->vm_pgoff);
  /* allocate and insert pages to fill the vma. */ 
  for(i=0; i < pages; i++) {
    page = alloc_page(GFP_KERNEL); // TODO IO RESERVE?
    if (!page) { 
      // TODO free previous pages
      printk(KERN_DEBUG "alloc_page failed\n");
      goto done;
    }
    addr = vma->vm_start+i*PAGE_SIZE;
    if (vm_insert_page(vma,addr,page) < 0) {
      // TODO free previous pages
      printk(KERN_DEBUG "vm_insert_page failed\n");
      goto done;
    }
    printk(KERN_DEBUG "inserted page %d at %p\n",i,(void*)addr);
    // TODO __free_page now, should be ok, since vm_insert_page incremented its
    // refcount. that way, upon munmap, refcount hits zer0, pages get freed 
    __free_page(page);
  }
  printk(KERN_DEBUG "completed inserting %lu pages\n", pages);
  
  rc = 0;

 done:
  return rc;
}

struct file_operations ops = {
  .read = _read,
  .write = _write,
  .open = _open,
  .release = _release,
  .mmap = _mmap
};

int __init chardev_init(void) {
  int rc;

  /* ask for a dynamic major */
  rc = alloc_chrdev_region(&c.dev, 0, NUM_MINORS, "memdriver");
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
