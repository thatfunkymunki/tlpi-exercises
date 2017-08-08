/* Exercise 5-7
 * Implement readv() and writev() using read(), write(), and suitable functions from the
 * malloc package (Section 7.1.2).
 */
#include "tlpi_hdr.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/uio.h>
/*  struct iovec {
 *    void *iov_ base; //start address of buffer
 *    ssize_t iov_len; //number of bytes to transfer to/from buffer
 *  }
 */
#define IOV_FOREACH(i, iovcnt) for(i=0;i<iovcnt;i++)
ssize_t _readv(int fd, const struct iovec *iov, int iovcnt);
//returns # of bytes read, 0 on eof, or -1 on error

ssize_t _writev(int fd, const struct iovec *iov, int iovcnt);
//returns number of bytes written or -1 on error


ssize_t _readv(int fd, const struct iovec *iov, int iovcnt){
  ssize_t numread=0;
  ssize_t *buffer;
  size_t bufsize=0;
  int i;
 
  //loop over the iov array
  IOV_FOREACH(i,iovcnt){
    //allocate sum(iov[n].iov_len) bytes for buffer
    bufsize+=iov[i].iov_len;
  }
  buffer = malloc(bufsize*sizeof(ssize_t));
  //read iov[n].iov_len number of bytes
  numread = read(fd,buffer,bufsize);
  //populate the vectors
  IOV_FOREACH(i,iovcnt){

  }
  return numread;
}
int main(int argc, char** argv){
  return 0;
}
