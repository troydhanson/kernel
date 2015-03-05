#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/fcntl.h>
 

int main(int argc, char * argv[]) {
  char *file = (argc > 1) ? argv[1] : "dev";
  int npages = (argc > 2) ? atoi(argv[2]) : 1;
  int fd,rc=-1;
  char *buf, *b, unused;
  size_t len = 4096 * npages; // FIXME getpagesz

  fprintf(stderr,"pid: %u\n", (int)getpid());

  if ( (fd = open(file, O_RDWR)) == -1) {
      fprintf(stderr,"can't open %s: %s\n", file, strerror(errno));
      goto done;
  }

  buf = mmap(0, len, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  if (buf == MAP_FAILED) {
    fprintf(stderr, "failed to mmap %s: %s\n", file, strerror(errno));
    goto done;
  }

  rc = 0;

  /* make the program block so we can examine it */

  fprintf(stderr,"press a key to exercise memory write\n");
  read(STDIN_FILENO,&unused,sizeof(unused));
  for(b=buf; b < buf+len; b++) *b=1;

  fprintf(stderr,"press a key to terminate\n");
  read(STDIN_FILENO,&unused,sizeof(unused));


 done:
  munmap(buf, len);
  close(fd);
  return rc;
}
