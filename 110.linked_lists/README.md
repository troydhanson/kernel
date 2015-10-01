There is a standard linked list implementation in the kernel.

    #include <linux/list.h>

This header defines the `list_head` structure:

```
   struct list_head {
     struct list_head *next, *prev;
   };
```

To make a structure into a linked list, embed this structure in it.
Declare a standalone `struct list_head` to serve as the list head.
The kernel defines several list manipulation primitives. A few of
them used in kex.c include `list_add`, `list_for_each`, `list_del`
and `list_for_each_safe`.

```
    % ./build.sh
    % sudo insmod kex.ko items=5
    % dmesg | tail
```

Clean up:

    % sudo rmmod kex
    % dmesg | tail
    % ./build.sh clean
