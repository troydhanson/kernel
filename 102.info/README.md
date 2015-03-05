This version of the kex module adds author and license info.
The compiled module retains this information. The modinfo
command displays it. The kernel also tracks whether a GPL-
compatible license applies to the module.

```
  % ./build.sh
  % modinfo kex.ko

filename:       /home/kernel/kex/./kex.ko
license:        Dual BSD/GPL
description:    Example of module information
author:         Troy D. Hanson
srcversion:     2249017AD0425E81C3EFBC9
depends:        
vermagic:       3.13.0-24-generic SMP mod_unload modversions 
```

A "Proprietary" license flags the kernel as tainted. Some
modules only expose their exported symbols to GPL code.

Clean up:

    % ./build.sh clean
