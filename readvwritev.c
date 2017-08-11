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
 *    void *iov_base; //start address of buffer
 *    ssize_t iov_len; //number of bytes to transfer to/from buffer
 *  }
 */
ssize_t _readv(int fd, const struct iovec *iov, int iovcnt);
//returns # of bytes read, 0 on eof, or -1 on error

ssize_t _writev(int fd, const struct iovec *iov, int iovcnt);
//returns number of bytes written or -1 on error


ssize_t _readv(int fd, const struct iovec *iov, int iovcnt){
  //find out how many bytes to read
  //allocate buffer to read that many bytes
  //read that many bytes
  //scatter that into iov **ensure that n < num of bytes read
  //  for each iovec i in iov
  //    for j=0..iov[i]->iov_len
  //      iov[i].iov_base[j]=buffer[n]
  //      n++
  
  int bufsize=0;
  char *buffer;
  ssize_t numread = 0;
  for(int i=0;i<iovcnt;i++){
    printf("adding %ld to bufsize\n",iov[i].iov_len);
    bufsize+=iov[i].iov_len;
  }
  buffer = malloc(bufsize);

  numread = read(fd, buffer, bufsize);

  for(int i=0, n=0;i<iovcnt && n<numread; i++){
    for(int j = 0; j < iov[i].iov_len && n<numread; j++){
      char *target = ((char*)iov[i].iov_base) + j;
      *target = buffer[n];
      n++;
    }
  }
  return numread;
  
}
int main(int argc, char** argv){
  struct iovec test[3];

  test[0].iov_len = 5;
  test[0].iov_base = malloc(5* sizeof(char));

  test[1].iov_len = 15;
  test[1].iov_base = malloc(15* sizeof(char));

  test[2].iov_len = 6;
  test[2].iov_base = malloc(6 * sizeof(char));

  int fd = open(argv[1], O_RDONLY);

  int numread = _readv(fd, test, 3);
  printf("num read: %d\n", numread);

  for(int i =0; i<3; i++){

    for(int j=0;j<test[i].iov_len; j++){
      printf("%d: %d: %x\n",i,j, *((char *)test[i].iov_base+j));

    }
  }
}
