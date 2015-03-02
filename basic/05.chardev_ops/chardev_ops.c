#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/fs.h>

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
  void *per_fd_state = NULL;
  /* we can store away some per fd state here. */
  f->private_data = per_fd_state;
  /* we can access the rw and blocking mode */
  printk(KERN_DEBUG "open for %c%c\n", ((f->f_mode & FMODE_READ)  ? 'r':'-'),
                                       ((f->f_mode & FMODE_WRITE) ? 'w':'-'));
  return 0;
}

/* kernel calls release when fd usage count reaches 0. e.g. if a process opens
 * a device, forks, fd usage count is now two; when both close, its released. */
int _release(struct inode *inode, struct file *f) {
  return 0;
}

/* read data into the userspace buf. some notes:
   1. buf is annotated with a leading __user to help us remember its userspace
   2. on real device a 'read' may block. 
   3. a driver can implement both blocking and non-blocking mode if desired.
   4. copy_to_user can sleep (e.g. if userspace buffer needs to be paged in).
   this example returns a zeroed buffer.
*/
char device_buf[100];
ssize_t _read(struct file *f, char __user *buf, size_t count, loff_t *offp) {
  ssize_t c = (count < sizeof(device_buf)) ? count : sizeof(device_buf);
  memset(device_buf,0,c);
  if (copy_to_user(buf, device_buf, c)) return -EFAULT;
  *offp += c;
  return c;
}

struct file_operations ops = {
  .read = _read,
  .write = NULL,
  .unlocked_ioctl = NULL,
  .open = _open,
  .release = _release
};

int __init chardev_init(void) {
  int rc;

  /* ask for a dynamic major */
  rc = alloc_chrdev_region(&c.dev, 0, NUM_MINORS, "chardev_ops");
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
