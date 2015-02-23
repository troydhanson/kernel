The "chardev" module registers a character device driver.

  % sudo insmod chardev.ko 

These days drivers like chardev dynamically get a major.
After the driver is loaded, its major and minor numbers
are exposed in /proc/devices. cf: /sys/dev,/sys/devices.

% cat /proc/devices | grep chardev
250 chardev

Then you can make a device node that uses this driver. 

  % sudo mknod dev c 250 0

  % ls -l dev
crw-r--r-- 1 root root 250, 0 Feb 18 09:30 dev

This driver has no "ops" yet, so it can't be used
for anything (e.g. "cat dev" fails). Clean up:

  % rm dev
  % sudo rmmod chardev
