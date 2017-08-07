/* EXERCISE 5-4
 * Implement dup() and dup2() using fcntl() and, where necessary, close().
 * (You may ignore the fact that dup2() and fcntl() return different errno
 * values for some error cases.) For dup2(), remember to handle the special
 * case where oldfd equals newfd. In this case, you should check whether
 * oldfd is valid, which can be done by, for example, if fcntl(oldfd,F_GETFL)
 * succeeds. if oldfd is not valid, then the function should return -1 with
 * errno set to EBADF.
 *
 */
#include "tlpi_hdr.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int _dup(int);
int _dup2(int, int);


int _dup(int oldfd){
  return fcntl(oldfd,F_DUPFD,0);
}
int _dup2(int oldfd, int newfd){
  close(newfd);
  return fcntl(oldfd,F_DUPFD,newfd);
}

int main (int argc, char** argv){


}
