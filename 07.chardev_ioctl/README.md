We add an ioctl handler to our driver that toggles
whether are counting bytes written to a descriptor.

There are several new things. We are using a structure
as the per-descriptor state rather than a simple long.
We have created a header file using some kernel macros
to define an ioctl that userspace (rig.c) can utilize.

  % ./build.sh
  % sudo insmod chardev_ioctl.ko 
  % cat /proc/devices | grep chardev_ioctl
250 chardev_ioctl
  % sudo mknod dev c 250 0
  % sudo chmod a+rw dev
  % ./rig
count: 5
count: 10
count: 10
count: 15

Notice that the third count remained at 10. This is
because we had toggled the driver mode sing our ioctl.

Clean up:

  % rm dev
  % sudo rmmod chardev_ioctl
  % ./build.sh clean
