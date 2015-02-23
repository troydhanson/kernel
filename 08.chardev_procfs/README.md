This driver adds an entry in procfs (/proc) using proc_create.
It exposes its name, and major and minor device numbers.
```
    % ./build.sh
    % sudo insmod chardev_procfs.ko 
    % cat /proc/chardev_procfs
chardev_procfs dev (250,0)
```
This is a simple use of procfs. A more complex driver can use
procfs for both input and output, and create multi-level directories.

Clean up:

    % sudo rmmod chardev_procfs
    % ./build.sh clean
