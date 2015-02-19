#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>

/* module information */
MODULE_AUTHOR("Troy D. Hanson");
MODULE_DESCRIPTION("Example of character device");
MODULE_LICENSE("Dual BSD/GPL");

#define NUM_MINORS 1

/* a global in the driver to keep state */
struct chardev_t {
  dev_t dev;        /* has major and minor bits */
  struct cdev cdev; /* has our ops, owner, etc */
} c;

static int _open(struct inode *inode, struct file *filp) {
  void *per_fd_state = NULL;
  /* we can store away some per fd state here. */
  filp->private_data = per_fd_state;
  /* we can access the rw and blocking mode */
  printk(KERN_DEBUG "open for %c%c\n", ((filp->f_mode & FMODE_READ) ? 'r':'-'),
                                      ((filp->f_mode & FMODE_WRITE) ? 'w':'-'));
  return 0;
}

/* kernel calls release when fd usage count reaches 0. e.g. if a process opens
 * a device, forks, fd usage count is now two; when both close, its released. */
static int _release(struct inode *inode, struct file *filp) {
  return 0;
}

struct file_operations ops = {
  .read = NULL,
  .write = NULL,
  .unlocked_ioctl = NULL,
  .open = _open,
  .release = NULL
};

int __init chardev_init(void) {
  int rc;

  /* ask for a dynamic major */
  rc = alloc_chrdev_region(&c.dev, 0, NUM_MINORS, "chardev");
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
