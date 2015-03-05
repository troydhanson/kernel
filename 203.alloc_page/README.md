This kex behaves the same as the previous one. Internally it uses alloc_page
instead of get_free_page.  This returns to us a struct page pointer. Then kex
gets its kernel virtual address using page_address(). 

```
    % ./build.sh
    % sudo insmod kex.ko 
    % cat /proc/devices | grep kex
250 kex
    % sudo mknod dev c 250 0
    % sudo chmod a+rw dev
    % dd if=/dev/urandom of=dev bs=4096 count=1
    % wc -c dev
4096 dev
```

Clean up:

    % rm dev
    % sudo rmmod kex
    % ./build.sh clean
