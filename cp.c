/* EXERCISE 4-2
 * Write a program like cp that, when used to copy a regular file that
 * contains holes (sequences of null bytes), also creates correspoding 
 * holes in the target file
 */
#include "tlpi_hdr.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

char buffer;
void usage(void);
int main (int argc, char** argv){

  if(argc<3){
    usage();
  }
  //printf("copying %s to %s\n", argv[1],argv[2]);
  int source = open(argv[1], O_RDONLY);
  if(source == -1){
    errExit("failed opening source");
  }
  int dest = open(argv[2], O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
  if(dest==-1){
    errExit("failed opening dest");
  }


  ssize_t num_read = 0;
  while((num_read = read(source, &buffer, 1)) > 0){ //read one byte at a time
    if(buffer == '\0'){ 
      //if we read a null byte from source, advance the fd
      lseek(dest, 1, SEEK_END);
    }
    else{
      write(dest, &buffer, 1);
    }
  }


  return 0;
}
void usage(){
  printf("cp <source> <destination>\n");
  exit(1);
}
