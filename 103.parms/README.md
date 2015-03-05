This version of kex shows how to add a parameter to a module.
Their types include byte, bool, int, long, charp, uint, etc.

```
    % ./build.sh
    % modinfo kex.ko 
parm:           verbose:set verbosity level (int)
```

Parameters can be given values when the module is loaded:

    % sudo insmod kex.ko verbose=1

Now the the module is loaded, its parameters appear in sysfs:

```
    % cd /sys/module/kex/parameters 
    % ls -l
-rw-r--r-- 1 root root 4096 Feb 15 18:23 verbose
    % cat verbose
1
    % # need to be root for the next line
    % echo 10 > verbose
    % cat verbose
10
```

Change back to the build directory, and clean up:

    % sudo rmmod kex
    % ./build.sh clean
