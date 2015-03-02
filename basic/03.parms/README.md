The "parms" module shows how to add a parameter to a module.
The parameter can be set at module load time.

    % insmod parms.ko verbose=1

You can also set module parameters at run time through sysfs.
This kernel filesystem is mounted at /sys. 

```
    % cd /sys/module/parms/parameters 
    % ls -l
-rw-r--r-- 1 root root 4096 Feb 15 18:23 verbose
```

Now you can view, or change, the module parameter:

```
  % cat verbose
  1
  % echo 10 > verbose
  % cat verbose
  10
```

You can also see module parameters in the modinfo list:

```
  % modinfo parms.ko 
...
parm:           verbose:set verbosity level (int)
```


Parameters can have these types:

  * byte
  * bool
  * invbool
  * charp
  * int
  * long 
  * short
  * uint
  * ulong
  * ushort

