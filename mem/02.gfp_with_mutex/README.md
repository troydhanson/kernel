This is a more fun variation of 01.get_free_page. This one keeps the
page inside the driver global state, in other words, one process
can write to it, and another process can read the buffer out. 

Since multiple processes may try to read or write the device at once,
this version adds the use of a mutex to lock the driver's page buffer.

```
    % ./build.sh 
    % sudo insmod memdriver.ko 
    % grep memdriver /proc/devices
250 memdriver
    % sudo mknod dev c 250 0
    % sudo chmod a+rw dev
    % echo hello > dev
    % cat dev
hello
```

You can write up to a full page (PAGE_SIZE is 4k on many systems) in
a single write operation; the driver buffers that in its page.

```
    % dd if=/dev/urandom of=dev bs=4096 count=1
    % wc -c dev
4096 dev
```

If we try to write more than a page worth of data, dd (the writing
process) has to loop to complete writing the full buffer. If we write
4097 bytes, dd has to make a second write for the final byte.

```
    % dd if=/dev/urandom of=dev bs=4097 count=1
    % wc -c dev
1 dev
```

Here we see that our driver only uses the last write as its measure
of how much data is available to read.

Clean up:

    % sudo rmmod memdriver
    % rm dev
    % ./build.sh clean
