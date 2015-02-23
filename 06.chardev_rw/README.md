The "chardev_rw" module supports both read and write.
This driver demonstrates keeping per-descriptor state.
The state it keeps is simply a count of bytes written.
When you read from it, it returns that count as a long.

  % sudo insmod chardev_rw.ko 

Determine the major:

% cat /proc/devices | grep chardev_rw
250 chardev_rw

Make the device node:

  % sudo mknod dev c 250 0

Try the Python helper script to write and read from it:

  % sudo ./rw.py
  5

Clean up:

  % rm dev
  % sudo rmmod chardev_rw
