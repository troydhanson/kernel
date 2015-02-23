This driver adds an entry in procfs (/proc) using proc_create.
It exposes its name, and major and minor device numbers.
```
    % ./build.sh
    % sudo insmod chardev_procfs.ko 
    % cat /proc/chardev_procfs
chardev_procfs dev (250,0)
```
This is a simple use of procfs. A driver can use procfs for input and
output, and create multi-level directories.  Note that proc_create has a
corresponding remove_proc_entry in the module cleanup function.

Clean up:

    % sudo rmmod chardev_procfs
    % ./build.sh clean
