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

void test_readv(int fd, const struct iovec  *iov, int iovcnt, ssize_t readv(int, const struct iovec *, int));
void test_writev(int fd, const struct iovec *iov, int iovcnt, ssize_t twritev(int, const struct iovec *, int));
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
  //  printf("adding %ld to bufsize\n",iov[i].iov_len);
    bufsize+=iov[i].iov_len;
  }
  buffer = malloc(bufsize);

  numread = read(fd, buffer, bufsize);

  for(int i=0, n=0;i<iovcnt && n<numread; i++){
    for(int j = 0; j < iov[i].iov_len && n<numread; j++, n++){
      char *target = ((char*)iov[i].iov_base) + j;
      *target = buffer[n];
    }
  }
  return numread;
  
}
ssize_t _writev(int fd, const struct iovec *iov, int iovcnt){
  //find out how many bytes to write
  //allocate buffer 
  //copy iov bytes into buffer
  //write buffer to fd

  int bufsize = 0;
  char *buffer;
  ssize_t  numcopied = 0;
  for(int i =0;i<iovcnt;i++){
    //printf("adding %ld to bufsize\n",iov[i].iov_len);
    bufsize+=iov[i].iov_len;

  }
  buffer= malloc(bufsize);

  for(int i=0; i<iovcnt; i++){
    //copy iov bytes into buffer
    for(int j=0; j < iov[i].iov_len; j++,numcopied++){
      if((buffer[numcopied] = *((char *)iov[i].iov_base+j) == EOF)){
          return numcopied;
  }
      //printf("%ld: %x\n", numcopied, buffer[numcopied]);
    }
  }

  return write(fd, buffer, numcopied);
  
}
int main(int argc, char** argv){
  struct iovec test[3];

  test[0].iov_len = 5;
  test[0].iov_base = malloc(5* sizeof(char));

  test[1].iov_len = 15;
  test[1].iov_base = malloc(15* sizeof(char));

  test[2].iov_len = 6;
  test[2].iov_base = malloc(6 * sizeof(char));

  printf("testing our readv\n");
  int fd = open(argv[1], O_RDONLY);

  test_readv(fd, test, 3, &_readv);

  printf("testing libc readv\n");
  fd = open(argv[1], O_RDONLY);
 
  test_readv(fd, test, 3, &readv);

 /* printf("testing our writev\n");
  int fd2 = open(argv[2], O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);

  test_writev(fd2, test, 3, &_writev);
*/
  printf("testing libc writev\n");
  int fd2 = open(argv[2], O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
  test_writev(fd2, test, 3, &writev);

}
void test_writev(int fd, const struct iovec *iov, int iovcnt, ssize_t twritev(int, const struct iovec *, int)){
  ssize_t written = twritev(fd, iov, iovcnt);
  printf("num written: %ld\n", written);

}
void test_readv(int fd, const struct iovec *iov, int iovcnt, ssize_t treadv(int, const struct iovec *, int)){
  ssize_t numread = treadv(fd, iov, iovcnt);
  printf("num read: %ld\n", numread);

  for(int i =0; i<iovcnt; i++){

    for(int j=0;j<iov[i].iov_len; j++){
      printf("%d: %d: %x\n",i,j, *((char *)iov[i].iov_base+j));

    }
  }
}
