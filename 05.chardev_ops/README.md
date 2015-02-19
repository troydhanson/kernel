The "chardev_ops" module adds ops to our device driver.
The ops are open, release (close), and read.

  % sudo insmod chardev.ko 

Determine the major.
Make the device node.
Read from it. 
Delete the device node.
