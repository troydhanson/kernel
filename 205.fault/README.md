This version of kex introduces one of the most interesting features of
memory management - page faulting. 

When the test rigs mmaps the device, this version of kex leaves the 
VMA's unpopulated. In other words, the rig has pages mapped into its
address space, for which no RAM backing exists.

When the rig process tries to read or write one of these pages, the MMU
looks up the corresponding physical RAM page in the page tables. Seeing
none it interrupts the kernel to decide how to handle the fault. In the
VMA kex has registered a fault handler, so the kernel calls it. Now kex
allocates a page of RAM to satisfy the fault.

The page fault, a transparent, on-demand VMA manipulation, is invisible
to the userspace application.

```
    % ./build.sh
    % sudo insmod kex.ko 
    % cat /proc/devices | grep kex
250 kex
    % sudo mknod dev c 250 0
    % sudo chmod a+rw dev
    % ./rig dev 1
pid: 24995
press a key to exercise memory write
```

While rig waits, use another terminal to examine its memory map. 
Notice RSS is 0 on the page mapped to the dev device- it's not in RAM:

```
    % pmap -x 24995
Address           Kbytes     RSS   Dirty Mode  Mapping
00007fdd3fff0000       4       0       0 rw-s- dev
```

In the rig window, press enter to cause it to write to this page. This
page fault causes kex to print a debug message, visible in dmesg:

```
    % dmesg | tail 
[30718.616879] fault on 00007fdd3fff0000 (userspace:1 write:1)
```

Now run pmap again. Notice that the page is now resident. It is also
dirty since rig wrote to the page after it was mapped.

```
    % pmap -x 24995
Address           Kbytes     RSS   Dirty Mode  Mapping
00007fdd3fff0000       4       4       4 rw-s- dev
```

Clean up:

    % rm dev
    % sudo rmmod kex
    % ./build.sh clean
