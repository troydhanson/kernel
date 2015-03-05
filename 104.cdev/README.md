This version of kex registers a character device driver.
Many modern drivers get their major numbers dynamically. 
This is what kex does. After it is loaded, its major and 
minor numbers are exposed in /proc/devices.

```
    % ./build.sh
    % sudo insmod kex.ko 
    % cat /proc/devices | grep kex
250 kex
```

Make a device node that uses this driver:

```
    % sudo mknod dev c 250 0
    % ls -l dev
crw-r--r-- 1 root root 250, 0 Feb 18 09:30 dev
```

This driver has no "ops" yet- it does not do anything.

Clean up:

```
    % ./build.sh clean
    % rm dev
    % sudo rmmod kex
```
