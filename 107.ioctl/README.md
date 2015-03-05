We add an ioctl handler to our kex driver. It toggles
whether kex is in "counting mode". In counting mode, it
counts bytes written to it (on a per descriptor basis).

There are several new things. We are using a structure
as the per-descriptor state rather than a simple long.
We have created a header file using some kernel macros
to define an ioctl that userspace (rig.c) can utilize.

```
    % ./build.sh
    % sudo insmod kex.ko 
    % cat /proc/devices | grep kex
250 kex
    % sudo mknod dev c 250 0
    % sudo chmod a+rw dev
```

Try out the test program:

```
    % ./rig
count: 5
count: 10
count: 10
count: 15
```

Notice that the third count remained at 10. This is
because it had toggled "counting mode" with the ioctl.

Clean up:

    % rm dev
    % sudo rmmod kex
    % ./build.sh clean
