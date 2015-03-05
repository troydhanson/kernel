This version of kex supports both read and write. This version also keeps
per-descriptor state.  In other words, each process that opens it gets a
state variable. The state is simply the count of bytes written to it on
that descriptor.  When we read from it, it returns that count as a long.

```
    % ./build.sh
    % sudo insmod kex.ko 
    % cat /proc/devices | grep kex
250 kex
    % sudo mknod dev c 250 0
    % sudo chmod a+rw dev
```

We use a helper script to open the device, write to it, and then read it:

```
    % sudo ./rw.py
5
```

Clean up:

    % rm dev
    % sudo rmmod kex
    % ./build.sh clean
