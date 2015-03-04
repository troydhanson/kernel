This module implements mmap and includes a test rig to exercise it.
When the test rig invokes mmap on the device, our driver uses alloc_page 
to get as many pages as the rig requested. It uses vm_insert_page on
each page to map it into rig's address space. These pages, while 
physically disjoint, appear as a single contiguous virtual memory area
(VMA) inside the rig address space. The VMA is visible using pmap:

```
    % ./build.sh 
    % sudo insmod memdriver.ko 
    % grep memdriver /proc/devices
250 memdriver
    % sudo mknod dev c 250 0
    % sudo chmod a+rw dev
    % ./rig dev 1
pid: 10195
press a key to exercise memory write
```

Leave the rig waiting. In another terminal run pmap. See the dev VMA:

```
    %  pmap -x 10195
Address           Kbytes     RSS   Dirty Mode  Mapping
00007fd0125b6000       4       4       0 rw-s- dev
```

It's 4kb because rig asked for one page (the final argument), which is 4kb.
Now we can ask rig to write to this memory. In the original terminal, press
enter.  Then re-run pmap in the second terminal:

```
Address           Kbytes     RSS   Dirty Mode  Mapping
00007fd0125b6000       4       4       4 rw-s- dev
```

Notice that the Dirty column now shows the mapped page is Dirty.  (TODO: is
this an MMU flag set on write to the page?). Press enter to terminate rig.

Clean up:

    % sudo rmmod memdriver
    % rm dev
    % ./build.sh clean
