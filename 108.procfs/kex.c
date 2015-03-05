#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>

/* module information */
MODULE_AUTHOR("Troy D. Hanson");
MODULE_DESCRIPTION("Example of character device");
MODULE_LICENSE("Dual BSD/GPL");

#define NUM_MINORS 1

/* a global to keep state. must be thread safe. */
struct kex_t {
  dev_t dev;            /* has major and minor bits */
  struct cdev cdev;     /* has our ops, owner, etc */

  /* static stuff we expose in procfs */
  char procfs_buf[100]; 
  int procfs_buf_len;

} c;

struct file_operations ops;

/******************************************************************************
 * procfs plumbing here 
 * we need to track whether they've consumed the buffer, otherwise repeated
 * read would keep returning the same buffer. use f->private_data to track.
 *****************************************************************************/
#define PROC_ENTRY_NAME "kex_info"
int proc_open(struct inode *inode, struct file *f) {
  unsigned long *len = (unsigned long*)&f->private_data;
  *len = c.procfs_buf_len; /* we need to write this much data */
  return 0;
}

/* this is invoked when userspace reads /proc/kex_info */
ssize_t proc_read(struct file *f, char __user *buf, size_t count, loff_t *offp) {
  unsigned long *len = (unsigned long*)&f->private_data;
  if (*len == 0) return 0; /* eof */
  if (count < *len) return -EINVAL;
  if (copy_to_user(buf, c.procfs_buf, *len)) return -EFAULT;
  *offp += *len;
  *len = 0;
  return c.procfs_buf_len;
}

/* file ops for our procfs entry */
struct file_operations procfs_ops = {
  .read = proc_read,
  .open = proc_open,
};

/******************************************************************************
 * module init below. 
 *****************************************************************************/
int __init kex_init(void) {
  int rc;
  struct proc_dir_entry *p;

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

  /* create the proc entry */
  p = proc_create(PROC_ENTRY_NAME, 0444, NULL, &procfs_ops);
  if (p==NULL) {
    printk(KERN_WARNING "create_proc_read_entry: failed\n");
    goto done;
  }
  c.procfs_buf_len = snprintf(c.procfs_buf, sizeof(c.procfs_buf), 
            PROC_ENTRY_NAME " dev (%d,%d)\n", MAJOR(c.dev), MINOR(c.dev));

  rc = 0;

 done:
  return rc ? -ENODEV : 0;
}

void __exit kex_cleanup(void) {
  cdev_del(&c.cdev);
  unregister_chrdev_region(c.dev, NUM_MINORS);
  remove_proc_entry(PROC_ENTRY_NAME, NULL);
}

module_init(kex_init);
module_exit(kex_cleanup);
