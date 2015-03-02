The "memdriver" module demonstrates get_free_page and free_page.
It associates a page with each open()'d descriptor on the device.
When a program writes to the descriptor, the data gets copied 
into the page (up to PAGE_SIZE bytes); when the program reads from
the descriptor it gets those bytes back.

In this driver we could have used kmalloc and kfree instead. With
get_free_page we get page-sized allocations; in contrast, kmalloc
is used for byte-sized (or physically contiguous) allocations.

```
    % ./build.sh
    % sudo insmod memdriver.ko 
    % grep memdriver /proc/devices
250 memdriver
    % mknod dev c 250 0
    % ./rw.py
```

Clean up:

    % sudo rmmod memdriver
    % rm dev
    % ./build.sh clean
