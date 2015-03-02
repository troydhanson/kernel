The "chardev_ops" module adds ops to our device driver.
The ops are open, release (close), and read.

    % sudo insmod chardev_ops.ko 

Determine the major:

```
% cat /proc/devices | grep chardev_ops
250 chardev_ops
```

Make the device node:

    % sudo mknod dev c 250 0

Read from it:

    % 2281  dd if=dev of=out bs=10 count=1
    % 2284  dd if=dev of=out bs=100 count=1
    % 2289  dd if=dev of=out bs=101 count=1

This driver returns 100 bytes max. The last dd only reads 100 bytes.

Clean up:

    % rm dev
    % sudo rmmod chardev_ops
