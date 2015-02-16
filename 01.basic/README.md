Basic kernel module
===================
Troy D. Hanson
February 2015

The "basic" module is a simple loadable kernel module. Its only purpose is to
show how to build and load a module. After you build it, you can load and 
unload it, and view its output with dmesg.

Install and locate the kernel-headers
-------------------------------------

The kernel headers are the bits required to build and link a kernel module (.ko).
On Debian derivates like Ubuntu and Mint, you can check if the kernel headers are
installed using the first command below and install it using the second command:

  % dpkg-query -s linux-headers-$(uname -r)
  % sudo apt-get install linux-headers-$(uname -r)

Set an environment variable to the installed kernel headers:

  % KERNELHEADERS=/lib/modules/$(uname -r)/build/

Note that $(uname -r) expands to the kernel version. E.g., 3.13.0-24-generic.

Build
-----
To build the module:

  % make -C ${KERNELHEADERS} SUBDIRS=$PWD modules

There are also make targets you can use including "clean" and "modules_install".

Try
---
To load the module:

  % sudo insmod basic.ko

To unload the module:

  % sudo rmmod basic

See the artifacts:

  % dmesg | tail -1

Produces:

  [65919.113771] basic module exit called


