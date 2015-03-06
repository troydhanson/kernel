This kex identifies which device inode, on which disk device, opened it.
This distinguishes two devices having the same major and minor from each
other. If they are on the same filesystem, their inodes would differ. If
they are on two different filesystems, their disk device major and minor
would differ (and in all likelihood their inode numbers as well). We can
treat the combination of disk device major and minor and device inode as
a "key" to tell whether two processes opened the same device node.

```
    % ./build.sh
    % sudo insmod kex.ko 
    % cat /proc/devices | grep kex
250 kex
    % sudo mknod dev c 250 0
    % ls -i dev
1075684 dev   
```

We used ls -i to see the inode number of our device node above. We open
the device, without writing any bytes to it, to provoke its log message:

```
    % cat /dev/null > dev
    % dmesg
[38890.769944] open(): inode 1075684 on dev (8,1)
```

Notice our driver's log message includes the inode number of our device.
It matches the one we expected. The log also says the device node inode 
resides on device (8,1). We can see that the current directory in which
the device node resides is on /dev/sda1 which is device (8,1) as shown:

```
    % df .
/dev/sda1       19478204 9067468   9398256  50% /
    % ls -l /dev/sda1
brw-rw---- 1 root disk 8, 1 Mar  2 16:54 /dev/sda1
```

Clean up:

    % rm dev
    % sudo rmmod kex
    % ./build.sh clean
