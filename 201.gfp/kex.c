#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/gfp.h>

/* module information */
MODULE_AUTHOR("Troy D. Hanson");
MODULE_DESCRIPTION("Example of character device");
MODULE_LICENSE("Dual BSD/GPL");

#define NUM_MINORS 1

/* a global to keep state. must be thread safe. */
struct chardev_t {
  dev_t dev;        /* has major and minor bits */
  struct cdev cdev; /* has our ops, owner, etc */
} c;

int _open(struct inode *inode, struct file *f) {
  unsigned long page = __get_free_page(GFP_KERNEL);
  if (!page) return -ENOMEM;
  printk(KERN_DEBUG " page virt addr %p\n", (void*)page);
  f->private_data = (void*)page;
  return 0;
}

int _release(struct inode *inode, struct file *f) {
  unsigned long page = (unsigned long)f->private_data;
  free_page(page);
  return 0;
}

ssize_t _read(struct file *f, char __user *buf, size_t count, loff_t *offp) {
  unsigned long page = (unsigned long)f->private_data;
  size_t sz = (count > PAGE_SIZE) ? PAGE_SIZE : count;
  if (copy_to_user(buf, (void*)page, sz)) return -EFAULT;
  *offp += sz;
  return sz;
}

ssize_t _write(struct file *f, const char __user *buf, size_t count, 
               loff_t *offp) {
  unsigned long page = (unsigned long)f->private_data;
  size_t sz = (count > PAGE_SIZE) ? PAGE_SIZE : count;
  if (copy_from_user((void*)page, buf, sz)) return -EFAULT;
  *offp += sz;
  return sz;
}

struct file_operations ops = {
  .read = _read,
  .write = _write,
  .unlocked_ioctl = NULL,
  .open = _open,
  .release = _release
};

int __init chardev_init(void) {
  int rc;

  /* ask for a dynamic major */
  rc = alloc_chrdev_region(&c.dev, 0, NUM_MINORS, "kex");
  if (rc) {
    printk(KERN_WARNING "alloc_chrdev_region: failed to get major\n");
    goto done;
  }

  /* init the struct cdev */
  cdev_init(&c.cdev, &ops);
  c.cdev.owner = THIS_MODULE;

  /* make device live */
  rc = cdev_add(&c.cdev, c.dev, NUM_MINORS);
  if (rc) {
    printk(KERN_WARNING "cdev_add: can't add device\n");
    unregister_chrdev_region(c.dev, NUM_MINORS);
    cdev_del(&c.cdev);
    goto done;
  }

  rc = 0;

 done:
  return rc ? -ENODEV : 0;
}

void __exit chardev_cleanup(void) {
  cdev_del(&c.cdev);
  unregister_chrdev_region(c.dev, NUM_MINORS);
}

module_init(chardev_init);
module_exit(chardev_cleanup);
