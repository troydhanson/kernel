The kex (kernel example) module initializes itself, then does nothing.  
Its debug messages are visible with dmesg when it's loaded and unloaded.

Prerequisite: kernel headers

The kernel headers are required to build and link a kernel module (.ko).
Debian derivates like Ubuntu and Mint can use the first command below to
check if they are installed. The second command installs them.

    % dpkg-query -s linux-headers-$(uname -r)
    % sudo apt-get install linux-headers-$(uname -r)

Set an environment variable to the kernel headers then build the module:

    % KERNELHEADERS=/lib/modules/$(uname -r)/build/
    % make -C ${KERNELHEADERS} SUBDIRS=$PWD modules

To simplify matters there is a script that does these two commands:

    % ./build.sh

Load and unload the module:

    % sudo insmod kex.ko
    % sudo rmmod kex

See the debug messages in the kernel log:

    % dmesg | tail 

Clean up:

    % ./build.sh clean
