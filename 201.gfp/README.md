This version of the kex module uses get_free_page and free_page.
It associates one page with each opened descriptor on the device.
When a program writes to the descriptor, kex copies the data into
the page, up to PAGE_SIZE bytes. When the program reads from the 
descriptor, kex copies those bytes back out to it.

```
    % ./build.sh
    % sudo insmod kex.ko 
    % cat /proc/devices | grep kex
250 kex
    % sudo mknod dev c 250 0
    % sudo chmod a+rw dev
    % ./rw.py
```

In this driver we could have used kmalloc and kfree instead. With
get_free_page we get page-sized allocations; in contrast, kmalloc
is used for byte-sized (or physically contiguous) allocations.

Clean up:

    % rm dev
    % sudo rmmod kex
    % ./build.sh clean
