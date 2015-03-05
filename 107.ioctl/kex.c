#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ioctl.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/fs.h>

#include "kex.h"

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

struct fd_state {
  long bytes_sunk; /* count of bytes written to fd */
  int counting;    /* 1 if we're in counting mode */
};

/* keep a count of bytes "sunk" (written) to this fd. */
int _open(struct inode *inode, struct file *f) {
  struct fd_state *s = (struct fd_state*)kmalloc(sizeof(*s), GFP_KERNEL);
  if (s==NULL) return -ENOMEM;
  s->bytes_sunk = 0;
  s->counting = 1;
  f->private_data = s;
  return 0;
}

int _release(struct inode *inode, struct file *f) {
  struct fd_state *s = (struct fd_state*)f->private_data;
  printk(KERN_DEBUG "sunk %lu bytes @ close\n", s->bytes_sunk);
  kfree(f->private_data);
  return 0;
}

/* return the number of bytes (as a long) ever written to this fd */
ssize_t _read(struct file *f, char __user *buf, size_t count, loff_t *offp) {
  struct fd_state *s = (struct fd_state*)f->private_data;
  if (count < sizeof(long)) return -EINVAL;
  if (copy_to_user(buf, &s->bytes_sunk, sizeof(long))) return -EFAULT;
  *offp += sizeof(long);
  return sizeof(long);
}

/* we simply add the byte count to the tally for this fd */
ssize_t _write(struct file *f, const char __user *buf, size_t count, 
               loff_t *offp) {
  struct fd_state *s = (struct fd_state*)f->private_data;
  if (s->counting) s->bytes_sunk += count;
  *offp += count;
  return count;
}

long _ioctl(struct file *f, unsigned int cmd, unsigned long arg) {
  struct fd_state *s = (struct fd_state*)f->private_data;
  long rc = -ENOTTY;

  if (_IOC_TYPE(cmd) != FOO_MAGIC) goto done; /* wrong ioctl for device */
  if (_IOC_NR(cmd) > FOO_NR) goto done;       /* ioctl is not supported */

  switch (cmd) {
    case FOO_IOCTL_TOGGLE:
      s->counting = s->counting ? 0 : 1;
      break;
    default:
      goto done;
      break;
  }

  rc = 0;

 done:
  return rc;
}

struct file_operations ops = {
  .read = _read,
  .write = _write,
  .unlocked_ioctl = _ioctl,
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
