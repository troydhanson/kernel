#include <linux/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include "kex.h"

/* 
 * This is a userspace test rig. 
 * It exercises our driver's read, write and ioctl.
 */

/* write bytes, then read the count */
int do_write(int fd) {
  int rc = -1, nr;
  long count;

  nr = write(fd,"hello",5);
  if (nr != 5) {
    fprintf(stderr,"write: %s\n", nr<0 ? strerror(errno) : "incomplete");
    goto done;
  }
  nr = read(fd,&count,sizeof(count));
  if (nr != sizeof(count)) {
    fprintf(stderr,"read: %s\n", nr<0 ? strerror(errno) : "incomplete");
    goto done;
  }
  fprintf(stderr,"count: %lu\n", count);

  rc = 0;

 done:
  return rc;
}

/* use ioctl to toggle whether driver counts bytes written */
int do_toggle(int fd) {
  int nr, rc = -1;

  nr = ioctl(fd,FOO_IOCTL_TOGGLE);
  if (nr < 0) {
    fprintf(stderr,"ioctl: %s\n", strerror(errno));
    goto done;
  }

  rc = 0;

 done:
  return rc;
}

int main(int argc, char *argv[]) {
  int rc = -1, fd;
  char *dev = (argc > 1) ? argv[1] : "dev";

  fd = open(dev,O_RDWR);
  if (fd < 0) {
    fprintf(stderr,"open '%s': %s\n", dev, strerror(errno));
    goto done;
  }

  if (do_write(fd)) goto done; /* prints 5 */
  if (do_write(fd)) goto done; /* prints 10 */

  if (do_toggle(fd)) goto done; /* stop counting */
  if (do_write(fd)) goto done; /* prints 10 */

  if (do_toggle(fd)) goto done; /* resume counting */
  if (do_write(fd)) goto done; /* prints 15 */

  close(fd);
  rc = 0;

 done:
  return rc;
}
