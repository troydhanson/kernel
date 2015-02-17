The "chardev" module registers a character device driver.

  % insmod chardev.ko 

These days drivers like chardev dynamically get a major.
After the driver is loaded, its major and minor numbers
are exposed in /sys/devices (and also in /proc/devices).

Then you can make a device node that uses this driver. 

  % mknod 

Now you can read and write data to this device.
