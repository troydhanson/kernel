These examples all implement a character driver as a loadable kernel module.

* basic/ shows module initialization, procfs, and r/w and ioctl ops
* mem/ shows usage of kernel memory allocation

TODO

* IRQ's
* timers, jiffies, HZ
* remap_pfn_range
* get_user_pages
* vm_insert_page
* udev device instantiation
* GPL_EXPORT_SYMBOLS, /proc/kallsyms
* ops: poll, mmap
