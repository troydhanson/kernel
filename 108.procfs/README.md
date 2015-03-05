This version of kex adds an entry in procfs (/proc). When this entry is
read, kex prints its name, and major and minor device numbers.

```
    % ./build.sh
    % sudo insmod kex.ko 
    % cat /proc/kex_info
kex_info dev (250,0)
```

Clean up:

    % sudo rmmod kex
    % ./build.sh clean
