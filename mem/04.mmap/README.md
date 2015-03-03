This module supports mmap and includes a test rig to exercise it.

```
    % ./build.sh 
    % sudo insmod memdriver.ko 
    % grep memdriver /proc/devices
250 memdriver
    % sudo mknod dev c 250 0
    % sudo chmod a+rw dev
    % ./rig
```

Clean up:

    % sudo rmmod memdriver
    % rm dev
    % ./build.sh clean
