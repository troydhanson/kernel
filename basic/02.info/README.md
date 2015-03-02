The "info" module build on the basic module. It adds author and license info.
This module information is visible using the modinfo command.

```
  % modinfo info.ko

filename:       /home/kernel_modules/info/./info.ko
license:        Dual BSD/GPL
description:    Example of module information
author:         Troy D. Hanson
srcversion:     2249017AD0425E81C3EFBC9
depends:        
vermagic:       3.13.0-24-generic SMP mod_unload modversions 
```

It is recommended to use a GPL-compatible license such as:

  * GPL
  * GPL v2
  * Dual BSD/GPL
  * Dual MIT/GPL

If the license is Proprietary, it flags the kernel as tainted.
You can view the taint status of a loaded module in sysfs, e.g.

  % cat /sys/module/info/taint
