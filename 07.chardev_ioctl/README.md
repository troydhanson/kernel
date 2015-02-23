We add an ioctl handler to chardev_ioctl. It toggles the
accumulation of byte counts. 

This driver demonstrates keeping per-descriptor state.
The state it keeps is simply a count of bytes written.
When you read from it, it returns that count as a long.

  % sudo insmod chardev_ioctl.ko 

Determine the major:

% cat /proc/devices | grep chardev_ioctl
250 chardev_ioctl

Make the device node:

  % sudo mknod dev c 250 0

Try the Python helper script to write and read from it:

  % sudo ./rw.py
  5

Clean up:

  % rm dev
  % sudo rmmod chardev_ioctl
