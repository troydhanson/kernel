#include <linux/ioctl.h>

#define FOO_MAGIC (0xff) /* anything we want */

/* let user apps use FOO_IOCTL_TOGGLE in ioctl */
#define FOO_IOCTL_TOGGLE _IO(FOO_MAGIC,0)

#define FOO_NR 1        /* number of ioctls */

