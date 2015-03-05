This version adds support for read() to the kex driver. 

```
    % ./build.sh
    % sudo insmod kex.ko 
    % cat /proc/devices | grep kex
250 kex
    % sudo mknod dev c 250 0
```

Read from the device:

```
    % dd if=dev of=/dev/null bs=101 count=1
100 bytes (100 B) copied, 0.000175226 s, 57.1 kB/s
```

Each process that opens this device can read up to 100 bytes 
from it; further reads return 0 to indicate EOF. This is how
this version of kex behaves. The dd above tried to read 101 
bytes but kex only made 100 bytes available.

Clean up:

    % rm dev
    % sudo rmmod kex
    % ./build.sh clean
