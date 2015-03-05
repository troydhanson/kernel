#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/fs.h>

/* module information */
MODULE_AUTHOR("Troy D. Hanson");
MODULE_DESCRIPTION("Example of character device");
MODULE_LICENSE("Dual BSD/GPL");

#define NUM_MINORS 1

/* a global to keep state. must be thread safe. */
struct kex_t {
  dev_t dev;        /* has major and minor bits */
  struct cdev cdev; /* has our ops, owner, etc */
} c;

/* keep a count of bytes "sunk" (written) to this fd. */
int _open(struct inode *inode, struct file *f) {
  long *bytes_sunk = (long*)kmalloc(sizeof(long), GFP_KERNEL);
  if (bytes_sunk==NULL) return -ENOMEM;
  *bytes_sunk = 0;
  f->private_data = bytes_sunk;
  return 0;
}

int _release(struct inode *inode, struct file *f) {
  long *bytes_sunk = (long*)f->private_data;
  printk(KERN_DEBUG "sunk %lu bytes @ close\n", *bytes_sunk);
  kfree(f->private_data);
  return 0;
}

/* return the number of bytes (as a long) ever written to this fd */
ssize_t _read(struct file *f, char __user *buf, size_t count, loff_t *offp) {
  if (count < sizeof(long)) return -EINVAL;
  if (copy_to_user(buf, f->private_data, sizeof(long))) return -EFAULT;
  *offp += sizeof(long);
  return sizeof(long);
}

/* we simply add the byte count to the tally for this fd */
ssize_t _write(struct file *f, const char __user *buf, size_t count, 
               loff_t *offp) {
  long *bytes_sunk = (long*)f->private_data;
  *bytes_sunk += count;
  *offp += count;
  return count;
}

struct file_operations ops = {
  .read = _read,
  .write = _write,
  .unlocked_ioctl = NULL,
  .open = _open,
  .release = _release
};

int __init kex_init(void) {
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

void __exit kex_cleanup(void) {
  cdev_del(&c.cdev);
  unregister_chrdev_region(c.dev, NUM_MINORS);
}

module_init(kex_init);
module_exit(kex_cleanup);
