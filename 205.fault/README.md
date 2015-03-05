This version of kex introduces one of the most interesting features of
memory management - page faulting. 

When the test rigs mmaps the device, this version of kex leaves the 
VMA's unpopulated. In other words, the rig has pages mapped into its
address space, for which no RAM backing exists.

When the rig process tries to read or write one of these pages, the MMU
looks up the corresponding physical RAM page in the page tables. Seeing
none it interrupts the kernel to decide how to handle the fault. In the
VMA kex has registered a fault handler, so the kernel calls it. Now kex
allocates a page of RAM to satisfy the fault.  The page fault, an on-
demand VMA manipulation, is invisible to the userspace application.

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

In the rig window, press enter to cause it to write to this page.
The page fault occurs as dmesg confirms. The page is now resident. 

```
    % pmap -x 24995
Address           Kbytes     RSS   Dirty Mode  Mapping
00007fdd3fff0000       4       4       4 rw-s- dev
```

As a side note, ps can show the page fault counts incurred by rig.
A minor fault is a RAM manipulation, which is the kind kex incurs.

    % ps -o pid,maj_flt,min_flt -C rig

Clean up:

    % rm dev
    % sudo rmmod kex
    % ./build.sh clean
