The "chardev" module registers a character device driver.
You can set its major/minor number as module parameters.

  % insmod chardev.ko major=1234 minor=1

Then you can make a device node that uses this driver. 

  % mknod 

Now you can read and write data to this device.
