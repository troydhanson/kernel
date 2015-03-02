This module is like 02.gfp_with_mutex except that it uses alloc_page
instead of get_free_page. This returns to us a struct page pointer.
We then get its kernel virtual address using page_address(). 

```
    % ./build.sh 
    % sudo insmod memdriver.ko 
    % grep memdriver /proc/devices
250 memdriver
    % sudo mknod dev c 250 0
    % sudo chmod a+rw dev
    % dd if=/dev/urandom of=dev bs=4096 count=1
    % wc -c dev
4096 dev
```

Clean up:

    % sudo rmmod memdriver
    % rm dev
    % ./build.sh clean
